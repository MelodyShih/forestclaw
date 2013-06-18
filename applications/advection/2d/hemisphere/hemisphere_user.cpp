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

#include "amr_forestclaw.H"
#include "amr_waveprop.H"
#include "hemisphere_user.H"

#ifdef __cplusplus
extern "C"
{
#if 0
}
#endif
#endif

void hemisphere_link_solvers(fclaw2d_domain_t *domain)
{
    fclaw2d_solver_functions_t* sf = get_solver_functions(domain);

    sf->use_single_step_update = fclaw_true;
    sf->use_mol_update = fclaw_false;

    sf->f_patch_setup              = &hemisphere_patch_setup;
    sf->f_patch_initialize         = &hemisphere_qinit;
    sf->f_patch_physical_bc        = &amr_waveprop_bc2;
    sf->f_patch_single_step_update = &hemisphere_update;

    amr_waveprop_link_to_clawpatch();
}

void hemisphere_patch_setup(fclaw2d_domain_t *domain,
                            fclaw2d_patch_t *this_patch,
                            int this_block_idx,
                            int this_patch_idx)
{
    // In case this is needed by the setaux routine
    set_block_(&this_block_idx);

    /* ----------------------------------------------------------- */
    // Global parameters
    const amr_options_t *gparms = get_domain_parms(domain);
    int mx = gparms->mx;
    int my = gparms->my;
    int mbc = gparms->mbc;

    /* ----------------------------------------------------------- */
    // Patch specific parameters
    ClawPatch *cp = get_clawpatch(this_patch);
    double xlower = cp->xlower();
    double ylower = cp->ylower();
    double dx = cp->dx();
    double dy = cp->dy();

    /* ----------------------------------------------------------- */
    // allocate space for the aux array
    amr_waveprop_define_auxarray(domain,cp);

    /* ----------------------------------------------------------- */
    // Pointers needed to pass to class setaux call, and other setaux
    // specific arguments
    double *aux;
    int maux;
    amr_waveprop_get_auxarray(domain,cp,&aux,&maux);

    int maxmx = mx;
    int maxmy = my;

    /* ----------------------------------------------------------- */
    /* Modified clawpack setaux routine that passes in mapping terms */
    double *xp = cp->xp();
    double *yp = cp->yp();
    double *zp = cp->zp();
    double *xd = cp->xd();
    double *yd = cp->yd();
    double *zd = cp->zd();
    double *area = cp->area();

    setaux_manifold_(maxmx,maxmy,mbc,mx,my,xlower,ylower,dx,dy,
                     maux,aux,xp,yp,zp,xd,yd,zd,area);
}

void hemisphere_qinit(fclaw2d_domain_t *domain,
                      fclaw2d_patch_t *this_patch,
                      int this_block_idx,
                      int this_patch_idx)
{
    // In case this is needed by the setaux routine
    set_block_(&this_block_idx);

    /* ----------------------------------------------------------- */
    // Global parameters
    const amr_options_t *gparms = get_domain_parms(domain);
    int mx = gparms->mx;
    int my = gparms->my;
    int mbc = gparms->mbc;
    int meqn = gparms->meqn;

    /* ----------------------------------------------------------- */
    // Patch specific parameters
    ClawPatch *cp = get_clawpatch(this_patch);
    double xlower = cp->xlower();
    double ylower = cp->ylower();
    double dx = cp->dx();
    double dy = cp->dy();

    /* ------------------------------------------------------- */
    // Pointers needed to pass to Fortran
    double* q = cp->q();

    double *aux;
    int maux;
    amr_waveprop_get_auxarray(domain,cp,&aux,&maux);

    // Other input arguments
    int maxmx = mx;
    int maxmy = my;

    double *xp = cp->xp();
    double *yp = cp->yp();
    double *zp = cp->zp();

    /* ------------------------------------------------------- */
    // Call to classic Clawpack 'qinit' routine.
    qinit_manifold_(maxmx,maxmy,meqn,mbc,mx,my,xlower,ylower,dx,dy,q,maux,aux,
                    xp,yp,zp);
}

void hemisphere_b4step2(fclaw2d_domain_t *domain,
                        fclaw2d_patch_t *this_patch,
                        int this_block_idx,
                        int this_patch_idx,
                        double t,
                        double dt)
{
    // In case this is needed by the setaux routine
    set_block_(&this_block_idx);

    /* ----------------------------------------------------------- */
    // Global parameters
    const amr_options_t *gparms = get_domain_parms(domain);
    int mx = gparms->mx;
    int my = gparms->my;
    int mbc = gparms->mbc;
    int meqn = gparms->meqn;

    /* ----------------------------------------------------------- */
    // Patch specific parameters
    ClawPatch *cp = get_clawpatch(this_patch);
    double xlower = cp->xlower();
    double ylower = cp->ylower();
    double dx = cp->dx();
    double dy = cp->dy();

    /* ------------------------------------------------------- */
    // Pointers needed to pass to Fortran
    double* q = cp->q();

    double *aux;
    int maux;
    amr_waveprop_get_auxarray(domain,cp,&aux,&maux);

    // Other input arguments
    int maxmx = mx;
    int maxmy = my;

    double *xd = cp->xd();
    double *yd = cp->yd();
    double *zd = cp->zd();

    /* ------------------------------------------------------- */
    // Classic call to b4step2(..)
    b4step2_manifold_(maxmx,maxmy,mbc,mx,my,meqn,q,xlower,ylower,dx,dy,t,dt,maux,aux,
                      xd,yd,zd);
}

double hemisphere_update(fclaw2d_domain_t *domain,
                         fclaw2d_patch_t *this_patch,
                         int this_block_idx,
                         int this_patch_idx,
                         double t,
                         double dt)
{
    hemisphere_b4step2(domain,this_patch,this_block_idx,this_patch_idx,t,dt);

    double maxcfl = amr_waveprop_step2(domain,this_patch,this_block_idx,this_patch_idx,t,dt);

    return maxcfl;
}



#ifdef __cplusplus
#if 0
{
#endif
}
#endif
