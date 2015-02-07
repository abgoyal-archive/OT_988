/* ------------------------------------------------------------------
 * Copyright (C) 1998-2009 PacketVideo
 * Copyright (c) 2009, Code Aurora Forum. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */
/*
    This PVA_FF_DecoderSpecificInfo Class that holds the Mpeg4 VOL header for the
	video stream
*/

#define __IMPLEMENT_EVRCDecoderSpecificInfo3GPP__

#include "evrcdecoderspecificinfo3gpp2.h"

#include "atom.h"
#include "atomutils.h"

#define EVRC_DECODER_SPECIFIC_SIZE 6

// Default constructor
PVA_FF_EVRCSpecificAtom::PVA_FF_EVRCSpecificAtom()
        : PVA_FF_Atom(FourCharConstToUint32('d', 'e', 'v', 'c'))
{
    _VendorCode = PACKETVIDEO_FOURCC;

    _frames_per_sample = 1;

    recomputeSize();
}

bool
PVA_FF_EVRCSpecificAtom::renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp)
{
    int32 rendered = 0;

    renderAtomBaseMembers(fp);
    rendered += getDefaultSize();

    // Render decoder specific info payload
    if (!PVA_FF_AtomUtils::render32(fp, _VendorCode))
    {
        return false;
    }
    rendered += 4;

    if (!PVA_FF_AtomUtils::render8(fp, _encoder_version))
    {
        return false;
    }
    rendered += 1;

    if (!PVA_FF_AtomUtils::render8(fp, _frames_per_sample))
    {
        return false;
    }
    rendered += 1;

    return true;
}

void
PVA_FF_EVRCSpecificAtom::recomputeSize()
{
    uint32 size = getDefaultSize();

    size += EVRC_DECODER_SPECIFIC_SIZE; // FOR DECODER SPECIFIC STRUCT

    _size = size;

    if (_pparent != NULL)
    {
        _pparent->recomputeSize();
    }
}

