      subroutine b4step2_manifold(mbc,mx,my,meqn,q,
     &            dx,dy,blockno,time,xd,yd,zd,dt,maux,aux)
      implicit none

      integer mbc, mx, my, meqn, maux, maxmx, maxmy
      double precision xlower, ylower, dx, dy, time, dt
      double precision q(1-mbc:maxmx+mbc,1-mbc:maxmy+mbc, meqn)
      double precision aux(1-mbc:maxmx+mbc,1-mbc:maxmy+mbc, maux)

      integer i, j
      double precision tperiod, pi2, vt, xll,yll, psi

      common /comvt/ tperiod,pi2
c
      if (tperiod .eq. 0.d0) then
c        # special case --- indication that velocities specified in
c        # setaux should be used for all time.
         return
      endif

      call compute_velocity_psi(mx,my,mbc,
     &      dx,dy,blockno,t,xd,yd,zd,aux,maux)

      vt = cos(pi2*(time+dt/2.d0)/tperiod)


      do i = 1-mbc,mx+mbc
         do j = 1-mbc,my+mbc
c           # multiply by time-factor:
            aux(i,j,1) = vt * aux(i,j,1)
            aux(i,j,2) = vt * aux(i,j,2)
         enddo
      enddo

      return
      end