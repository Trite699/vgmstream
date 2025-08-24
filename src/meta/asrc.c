#include "meta.h"
#include "../coding/coding.h"

VGMSTREAM* init_vgmstream_asrc(STREAMFILE* sf) {
    VGMSTREAM* vgmstream = NULL;
    int channels, sample_rate, num_samples;

    /* check extension */
    if (!check_extensions(sf, "asrc"))
        return NULL;

    /* check magic (example, replace with real one) */
    if (read_32bitBE(0x00, sf) != 0x41535243) /* "ASRC" */
        return NULL;

    channels     = read_32bitLE(0x04, sf);
    sample_rate  = read_32bitLE(0x08, sf);
    num_samples  = read_32bitLE(0x0C, sf);

    vgmstream = allocate_vgmstream(channels, 0);
    if (!vgmstream) return NULL;

    vgmstream->sample_rate = sample_rate;
    vgmstream->num_samples = num_samples;
    vgmstream->coding_type = coding_PCM16LE;
    vgmstream->layout_type = layout_interleave;
    vgmstream->interleave_block_size = 0x02; /* per sample */

    vgmstream->meta_type = meta_ASRC;

    if (!vgmstream_open_stream(vgmstream, sf, 0x10))
        goto fail;

    return vgmstream;

fail:
    close_vgmstream(vgmstream);
    return NULL;
}
