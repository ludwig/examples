       subroutine gridloop2(a, xcoor, ycoor, nx, ny, func1)
       integer nx, ny
       real*8 a(0:nx-1,0:ny-1), xcoor(0:nx-1), ycoor(0:ny-1), func1
       external func1
Cf2py  intent(out) a
Cf2py  intent(in) xcoor
Cf2py  intent(in) ycoor
Cf2py  depend(nx,ny) a

       integer i,j
       real*8 x,y
       do j = 0, ny-1
          y = ycoor(j)
          do i = 0, nx-1
             x = xcoor(i)
             a(i,j) = func1(x,y)
          end do
       end do
       return
       end
