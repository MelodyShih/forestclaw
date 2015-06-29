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

#ifndef PWCONST_USER_H
#define PWCONST_USER_H

#include "fclaw2d_clawpatch.h"
#include "fc2d_clawpack46.h"

#ifdef __cplusplus
extern "C"
{
#if 0
}
#endif
#endif

void pwconst_tag4refinement_(const int& mx,const int& my,const int& mbc,
                            const int& meqn, const double& xlower,
                            const double& ylower, const double& dx, const double& dy,
                            double q[], const int& init_flag, int& tag_patch);

void pwconst_tag4coarsening_(const int& mx,const int& my,const int& mbc,
                            const int& meqn,const double& xlower,
                            const double& ylower, const double& dx, const double& dy,
                            double qcoarsened[], int& tag_patch);

/* These are here in case the user wants to change the output */
void pwconst_write_tfile_(const int* iframe,const double* time,
                          const int* mfields,const int* ngrids);

void pwconst_write_qfile_(const int* mx,        const int* my,
                          const int* meqn,      const int* mbc,
                          const double* xlower, const double* ylower,
                          const double* dx,     const double* dy,
                          double q[],           const int* iframe,
                          const int* patch_num, const int* level,
                          const int* blockno,   const int* mpirank);

void pwconst_link_solvers(fclaw2d_domain_t *domain);

fclaw_bool pwconst_patch_tag4refinement(fclaw2d_domain_t *domain,
                                          fclaw2d_patch_t *this_patch,
                                          int this_block_idx, int this_patch_idx,
                                          int initflag);

fclaw_bool pwconst_patch_tag4coarsening(fclaw2d_domain_t *domain,
                                          fclaw2d_patch_t *this_patch,
                                          int blockno,
                                          int patchno);

fclaw2d_map_context_t* fclaw2d_map_new_nomap();

#ifdef __cplusplus
#if 0
{
#endif
}
#endif

#endif