/*
Copyright (c) 2012 Carsten Burstedde, Donna Calhoun
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef FCLAW2D_VTK_H
#define FCLAW2D_VTK_H

#include "forestclaw2d.h"
#include <p4est_wrap.h>

#ifdef __cplusplus
extern "C"
{
#if 0
}                               /* need this because indent is dumb */
#endif
#endif

/** Callback to access/compute patch data for visualization.
 * \param [in,out] a    The callback should write into this memory.
 *                      For coordinate locations, it holds
 *                      (my + 1) * (mx + 1) * 3 floats.
 *                      For patch data, it holds my * mx * meqn floats.
 *                      The vector index changes fastest, then mx, then my
 *                      slowest.
 */
typedef void
    (*fclaw2d_vtk_patch_data_t) (fclaw2d_domain_t * domain,
                                 fclaw2d_patch_t * this_patch,
                                 int this_block_idx, int this_patch_idx,
                                 char *a);

/** Write a file in VTK format for the whole domain in parallel.
 * \return          0 if successful, negative otherwise.
 *                  Collective with identical value on all ranks.
 */
int fclaw2d_vtk_write_file (fclaw2d_domain_t * domain, const char *basename,
                            int mx, int my, int meqn,
                            fclaw2d_vtk_patch_data_t coordinate_cb,
                            fclaw2d_vtk_patch_data_t value_cb);

#ifdef __cplusplus
#if 0
{                               /* need this because indent is dumb */
#endif
}
#endif

#endif /* !FCLAW2D_VTK_H */