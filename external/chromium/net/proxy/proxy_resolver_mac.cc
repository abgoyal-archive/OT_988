// Copyright (c) 2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/proxy/proxy_resolver_mac.h"

#include <CoreFoundation/CoreFoundation.h>

#include "base/logging.h"
#include "base/mac_util.h"
#include "base/scoped_cftyperef.h"
#include "base/string_util.h"
#include "base/sys_string_conversions.h"
#include "net/base/net_errors.h"
#include "net/proxy/proxy_info.h"
#include "net/proxy/proxy_server.h"

namespace {

// Utility function to map a CFProxyType to a ProxyServer::Scheme.
// If the type is unknown, returns ProxyServer::SCHEME_INVALID.
net::ProxyServer::Scheme GetProxyServerScheme(CFStringRef proxy_type) {
  if (CFEqual(proxy_type, kCFProxyTypeNone))
    return net::ProxyServer::SCHEME_DIRECT;
  if (CFEqual(proxy_type, kCFProxyTypeHTTP))
    return net::ProxyServer::SCHEME_HTTP;
  if (CFEqual(proxy_type, kCFProxyTypeSOCKS)) {
    // We can't tell whether this was v4 or v5. We will assume it is
    // v5 since that is the only version OS X supports.
    return net::ProxyServer::SCHEME_SOCKS5;
  }
  return net::ProxyServer::SCHEME_INVALID;
}

// Callback for CFNetworkExecuteProxyAutoConfigurationURL. |client| is a pointer
// to a CFTypeRef.  This stashes either |error| or |proxies| in that location.
void ResultCallback(void* client, CFArrayRef proxies, CFErrorRef error) {
  DCHECK((proxies != NULL) == (error == NULL));

  CFTypeRef* result_ptr = reinterpret_cast<CFTypeRef*>(client);
  DCHECK(result_ptr != NULL);
  DCHECK(*result_ptr == NULL);

  if (error != NULL) {
    *result_ptr = CFRetain(error);
  } else {
    *result_ptr = CFRetain(proxies);
  }
  CFRunLoopStop(CFRunLoopGetCurrent());
}

}  // namespace

namespace net {

// Gets the proxy information for a query URL from a PAC. Implementation
// inspired by http://developer.apple.com/samplecode/CFProxySupportTool/
int ProxyResolverMac::GetProxyForURL(const GURL& query_url,
                                     ProxyInfo* results,
                                     CompletionCallback* /*callback*/,
                                     RequestHandle* /*request*/,
                                     LoadLog* load_log) {
  scoped_cftyperef<CFStringRef> query_ref(
      base::SysUTF8ToCFStringRef(query_url.spec()));
  scoped_cftyperef<CFURLRef> query_url_ref(
      CFURLCreateWithString(kCFAllocatorDefault,
                            query_ref.get(),
                            NULL));
  if (!query_url_ref.get())
    return ERR_FAILED;
  scoped_cftyperef<CFStringRef> pac_ref(
      base::SysUTF8ToCFStringRef(pac_url_.spec()));
  scoped_cftyperef<CFURLRef> pac_url_ref(
      CFURLCreateWithString(kCFAllocatorDefault,
                            pac_ref.get(),
                            NULL));
  if (!pac_url_ref.get())
    return ERR_FAILED;

  // Work around <rdar://problem/5530166>. This dummy call to
  // CFNetworkCopyProxiesForURL initializes some state within CFNetwork that is
  // required by CFNetworkExecuteProxyAutoConfigurationURL.

  CFArrayRef dummy_result = CFNetworkCopyProxiesForURL(query_url_ref.get(),
                                                       NULL);
  if (dummy_result)
    CFRelease(dummy_result);

  // We cheat here. We need to act as if we were synchronous, so we pump the
  // runloop ourselves. Our caller moved us to a new thread anyway, so this is
  // OK to do. (BTW, CFNetworkExecuteProxyAutoConfigurationURL returns a
  // runloop source we need to release despite its name.)

  CFTypeRef result = NULL;
  CFStreamClientContext context = { 0, &result, NULL, NULL, NULL };
  scoped_cftyperef<CFRunLoopSourceRef> runloop_source(
      CFNetworkExecuteProxyAutoConfigurationURL(pac_url_ref.get(),
                                                query_url_ref.get(),
                                                ResultCallback,
                                                &context));
  if (!runloop_source)
    return ERR_FAILED;

  const CFStringRef private_runloop_mode =
      CFSTR("org.chromium.ProxyResolverMac");

  CFRunLoopAddSource(CFRunLoopGetCurrent(), runloop_source.get(),
                     private_runloop_mode);
  CFRunLoopRunInMode(private_runloop_mode, DBL_MAX, false);
  CFRunLoopRemoveSource(CFRunLoopGetCurrent(), runloop_source.get(),
                        private_runloop_mode);
  DCHECK(result != NULL);

  if (CFGetTypeID(result) == CFErrorGetTypeID()) {
    // TODO(avi): do something better than this
    CFRelease(result);
    return ERR_FAILED;
  }
  DCHECK(CFGetTypeID(result) == CFArrayGetTypeID());
  scoped_cftyperef<CFArrayRef> proxy_array_ref((CFArrayRef)result);

  // This string will be an ordered list of <proxy-uri> entries, separated by
  // semi-colons. It is the format that ProxyInfo::UseNamedProxy() expects.
  //    proxy-uri = [<proxy-scheme>"://"]<proxy-host>":"<proxy-port>
  // (This also includes entries for direct connection, as "direct://").
  std::string proxy_uri_list;

  CFIndex proxy_array_count = CFArrayGetCount(proxy_array_ref.get());
  for (CFIndex i = 0; i < proxy_array_count; ++i) {
    CFDictionaryRef proxy_dictionary =
        (CFDictionaryRef)CFArrayGetValueAtIndex(proxy_array_ref.get(), i);
    DCHECK(CFGetTypeID(proxy_dictionary) == CFDictionaryGetTypeID());

    // The dictionary may have the following keys:
    // - kCFProxyTypeKey : The type of the proxy
    // - kCFProxyHostNameKey
    // - kCFProxyPortNumberKey : The meat we're after.
    // - kCFProxyUsernameKey
    // - kCFProxyPasswordKey : Despite the existence of these keys in the
    //                         documentation, they're never populated. Even if a
    //                         username/password were to be set in the network
    //                         proxy system preferences, we'd need to fetch it
    //                         from the Keychain ourselves. CFProxy is such a
    //                         tease.
    // - kCFProxyAutoConfigurationURLKey : If the PAC file specifies another
    //                                     PAC file, I'm going home.

    CFStringRef proxy_type =
        (CFStringRef)mac_util::GetValueFromDictionary(proxy_dictionary,
                                                      kCFProxyTypeKey,
                                                      CFStringGetTypeID());
    ProxyServer proxy_server = ProxyServer::FromDictionary(
        GetProxyServerScheme(proxy_type),
        proxy_dictionary,
        kCFProxyHostNameKey,
        kCFProxyPortNumberKey);
    if (!proxy_server.is_valid())
      continue;

    if (!proxy_uri_list.empty())
      proxy_uri_list += ";";
    proxy_uri_list += proxy_server.ToURI();
  }

  if (!proxy_uri_list.empty())
    results->UseNamedProxy(proxy_uri_list);
  // Else do nothing (results is already guaranteed to be in the default state).

  return OK;
}

}  // namespace net
