s = 1e-2;
axis([-s 1+s -s 1+s])
daspect([1 1 1]);
axis off;
%yrbcolormap;

cv = 0.1:0.1:0.9;
showpatchborders;
setpatchborderprops(1:7,'linewidth',2);
showgridlines(1:3);
hidegridlines;
%caxis([0 1]);

view(2);

NoQuery = 0;
prt = false;
if (prt)
  filename = 'swirl000.png';
  str = num2str(Frame);
  len = length(str);
  filename(8-len+1:8) = str;
  pstr = ['print -dpng ',filename];
  disp(pstr);
  eval(pstr);
end;

clear afterframe;