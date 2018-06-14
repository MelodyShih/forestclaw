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

#include "fc2d_geoclaw_gauges_default.h"

#include "fc2d_geoclaw_gauges.h"
#include "fc2d_geoclaw_fort.h"

#include "fc2d_geoclaw_options.h"

#include <fclaw2d_clawpatch.h>
#include <fclaw2d_clawpatch_options.h>

#include <fclaw2d_global.h>

#ifdef __cplusplus
extern "C"
{
#if 0
}
#endif
#endif

typedef struct geoclaw_user
{
    int level;
    double tcurr;
    double qvar[3];  /* Store h, hu, hv */
    double avar[1];  /* Store bathymetry */
} geoclaw_user_t;


void geoclaw_read_gauges_data_default(fclaw2d_global_t *glob, 
                                      fc2d_geoclaw_gauge_t **gauges,
                                      int *num_gauges)
{
    /* Idea is that we may only need to change this file when updating to newer
       geoclaw code */
/* Sample gauges.data file */
/*
########################################################
### DO NOT EDIT THIS FILE:  GENERATED AUTOMATICALLY ####
### To modify data, edit  setrun.py                 ####
###    and then "make .data"                        ####
########################################################

18                   =: ngauges             
   1   8.6001000000e+01   1.0000000000e-03   0.000000e+00   1.000000e+10
   2   8.6876000000e+01   1.0000000000e-03   0.000000e+00   1.000000e+10
   3   8.7751000000e+01   1.0000000000e-03   0.000000e+00   1.000000e+10
   ....
*/

    FILE *f_gauges_data;
    int max_line_len = 200;  /* Maximum line length in file  gauges.data */
    char *line, *next;  
    double *xc,*yc,*t1,*t2,*min_time_increment;
    int *num;
    int i;

    line = FCLAW_ALLOC(char,max_line_len);

    f_gauges_data = fopen("gauges.data","r");

    /* Read header */
    for(i = 0; i < 5; i++)
    {
        /* Read lines of each header file.  Assume 100 chars in each line */
        fgets(line, max_line_len, f_gauges_data);
        if (line[0] != '#')
        {
            fclaw_global_essentialf("\n\n");
            fclaw_global_essentialf("read_gauges_data : Error reading header.\n");
            exit(0);
        }
    }
    /* blank line */
    fgets(line,max_line_len, f_gauges_data); 

    /* Read number of gauges */
    fgets(line,max_line_len, f_gauges_data);
    *num_gauges = strtod(line,NULL);

    if (*num_gauges == 0)
    {
        *gauges = NULL;
    }
    else
    {
        fc2d_geoclaw_gauge_allocate(glob,*num_gauges,gauges);
        fc2d_geoclaw_gauge_t *g = *gauges;

        num = FCLAW_ALLOC(int,   *num_gauges);
        xc  = FCLAW_ALLOC(double,*num_gauges);
        yc  = FCLAW_ALLOC(double,*num_gauges);
        t1  = FCLAW_ALLOC(double,*num_gauges);
        t2  = FCLAW_ALLOC(double,*num_gauges);
        min_time_increment = FCLAW_ALLOC(double,*num_gauges);

        /* Read gauge info */
        for(i = 0; i < *num_gauges; i++)
        {                        
            fgets(line,max_line_len, f_gauges_data);
            num[i] = strtod(line,&next);
            xc[i] = strtod(next,&next);
            yc[i] = strtod(next,&next);
            t1[i] = strtod(next,&next);
            t2[i] = strtod(next,NULL);
        }

        /* Skip a bunch of lines to get to min_time_increment */
        for(i = 0; i < 8; i++)
        {
            fgets(line,max_line_len,f_gauges_data);        
        }

        /* Read minimum time increment */
        fgets(line,max_line_len,f_gauges_data);
        min_time_increment[0] = strtod(line,&next);
        for(i = 1; i < *num_gauges; i++)
        {
            min_time_increment[i] = strtod(next,&next);
        }

        for(i = 0; i < *num_gauges; i++)
        {
            fc2d_geoclaw_gauge_set_data(glob,&g[i],num[i],
                                        xc[i],yc[i],t1[i],t2[i],
                                        min_time_increment[i]);
        }

        FCLAW_FREE(num);
        FCLAW_FREE(xc);
        FCLAW_FREE(yc);
        FCLAW_FREE(t1);
        FCLAW_FREE(t2);
        FCLAW_FREE(min_time_increment);

    }   /* End of num_gauges > 0 loop */

    /* Finish up */
    fclose(f_gauges_data);    
    FCLAW_FREE(line);
}

/* This function can be virtualized so the user can specify their 
   gauge output */

void geoclaw_create_gauge_files_default(fclaw2d_global_t *glob, 
                                        fc2d_geoclaw_gauge_t *gauges,
                                        int num_gauges)
{
    int num;
    double xc,yc,t1,t2;

    /* -----------------------------------------------------
    Open output gauge files and add header information
    ----------------------------------------------------- */
    char filename[15];    /* gaugexxxxx.txt  + EOL */
    FILE *fp;

    int num_eqns = 4;  /* meqn + 1 (h, hu, hv, eta) */
    for (int i = 0; i < num_gauges; i++)
    {
        fc2d_geoclaw_gauge_get_data(glob,&gauges[i],&num, &xc, &yc, &t1, &t2);

        sprintf(filename,"gauge%05d.txt",num);
        fp = fopen(filename, "w");
        /* This must have exactly the spacing indicated below. 
           See line 99 of $CLAW/pyclaw/src/pyclaw/gauges.py */
        fprintf(fp, "# gauge_id= %5d location=( %17.10e %17.10e ) num_eqn= %2d\n",
                num, xc, yc, num_eqns);

        fprintf(fp, "# Columns: level time h    hu    hv    eta\n");
        fclose(fp);
    }
}

void geoclaw_gauge_update_default(fclaw2d_global_t* 
                                  glob, fclaw2d_block_t* block,
                                  fclaw2d_patch_t* patch, 
                                  int blockno, int patchno,
                                  double tcurr, fc2d_geoclaw_gauge_t *g)
{
    int mx,my,mbc,meqn,maux;
    double xlower,ylower,dx,dy;
    double *q, *aux;
    double qvar[3], avar[1];  /* q[meqn] */
    int num;
    double xc,yc, t1, t2;

    int m;

    fclaw2d_clawpatch_grid_data(glob,patch,&mx,&my,&mbc,
                                &xlower,&ylower,&dx,&dy);

    fc2d_geoclaw_gauge_get_data(glob,g,&num, &xc, &yc, &t1, &t2);

    FCLAW_ASSERT(xc >= xlower && xc <= xlower + mx*dx);
    FCLAW_ASSERT(yc >= ylower && yc <= ylower + my*dy);

    fclaw2d_clawpatch_soln_data(glob,patch,&q,&meqn);
    fclaw2d_clawpatch_aux_data(glob,patch,&aux,&maux);

    /* Interpolate q variables and aux variables (bathy only for now)
       to gauge location */
    FC2D_GEOCLAW_UPDATE_GAUGE(&mx,&my,&mbc,&meqn,&xlower,&ylower,
                              &dx,&dy,q,&maux,aux,&xc,&yc,
                              qvar,avar);
                
    /* Store qvar, avar in gauge buffers;  Anything stored will be printed
       in print_buffers */
    geoclaw_user_t *guser = FCLAW_ALLOC(geoclaw_user_t,1);
    guser->level = patch->level;
    guser->tcurr = tcurr;
    for(m = 0; m < meqn; m++)
    {
        guser->qvar[m] = qvar[m];
    }
    guser->avar[0] = avar[0];   /* Just store bathymetry for now */
    fc2d_geoclaw_gauge_set_buffer_entry(glob,g,guser);
}

void geoclaw_print_gauges_default(fclaw2d_global_t *glob, 
                                  fc2d_geoclaw_gauge_t *gauge) 
{
    int k, kmax;
    geoclaw_user_t **gauge_buffer;

    char filename[15];  /* gaugexxxxx.txt + EOL character */
    FILE *fp;

    /* This assumes on buffers be organized as an array; entries
       start at 0 and with kmax-1 */
    fc2d_geoclaw_gauge_get_buffer(glob,gauge,&kmax,(void***) &gauge_buffer);

    sprintf(filename,"gauge%05d.txt",gauge->num);
    fp = fopen(filename, "a");
    for(k = 0; k < kmax; k++)
    {
        geoclaw_user_t *guser = gauge_buffer[k];

        double eta = guser->qvar[0] + guser->avar[0];
        eta = abs(eta) < 1e-99 ? 0 : eta; /* For reading in Matlab */
        fprintf(fp, "%5d %15.7e %15.7e %15.7e %15.7e %15.7e\n",
                guser->level, guser->tcurr,
                guser->qvar[0],guser->qvar[1],
                guser->qvar[2],eta);

        FCLAW_FREE(guser);
    }
    fclose(fp);
}

#ifdef __cplusplus
#if 0
{
#endif
}
#endif

