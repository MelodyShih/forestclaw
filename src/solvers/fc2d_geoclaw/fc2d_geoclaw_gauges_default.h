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

#ifndef FC2D_GEOCLAW_GAUGES_DEFAULT_H
#define FC2D_GEOCLAW_GAUGES_DEFAULT_H

#ifdef __cplusplus
extern "C"
{
#if 0
}
#endif
#endif

struct fclaw2d_global;
struct fc2d_geoclaw_gauge;

void geoclaw_read_gauges_data_default(struct fclaw2d_global *glob, 
                              struct fc2d_geoclaw_gauge **gauges, 
                              int *num);

void geoclaw_create_gauge_files_default(struct fclaw2d_global *glob, 
                                struct fc2d_geoclaw_gauge *gauges, 
                                int num_gauges);

void geoclaw_gauge_update_default(struct fclaw2d_global* glob, 
                                  struct fclaw2d_block* block,
                                  struct fclaw2d_patch* patch, 
                                  int blockno, int patchno,
                                  double tcurr, struct fc2d_geoclaw_gauge *g);

#if 1
void geoclaw_store_gauge_vars_default(struct fclaw2d_global *glob, 
                                      int level, double tcurr,
                                      double* qvar, double *avar,
                                      struct fc2d_geoclaw_gauge *gauge);
#endif                                      

void geoclaw_print_gauges_default(struct fclaw2d_global *glob, 
                                  struct fc2d_geoclaw_gauge *gauge);

void geoclaw_gauge_destroy_default(struct fclaw2d_global *glob, 
                                   struct fc2d_geoclaw_gauge *g);



#ifdef __cplusplus
#if 0
{
#endif
}
#endif

#endif