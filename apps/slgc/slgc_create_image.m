%
% Run this file with: matlab -nodisplay -nojvm -r foo
%

% data goes here
fid = fopen('seven.dat', 'rt');
if (fid < 0)
    error('Could not open file "seven.dat"');
end;
x = fscanf(fid, '%d', [7,24*12])';
fclose(fid);


% plot it w/ pcolor
cfigure(30,20);
y = [x zeros(length(x),1)];
h = pcolor(y);
colormap(hot)
colorbar
axis ij

%title(['Weekly Activity of Second Life Go Center', sprintf('\n'), 'Aug 19-25, 2008 (Pacific Time)'])
title(['Weekly Activity of Second Life Go Center'])
set(h, 'LineStyle', 'none')

set(gca, 'yticklabel', {'12 am',' 3 am',' 6 am',' 9 am','12 pm',' 3 pm',' 6 pm',' 9 pm',''})
set(gca, 'ytick', [1 37 73 109 145 181 217 253 288])

%set(gca, 'xticklabel', {'Tue','Wed','Thu','Fri','Sat','Sun','Mon'})
%set(gca, 'xticklabel', {'Sun', 'Mon', 'Tue','Wed','Thu','Fri','Sat'})
set(gca, 'xticklabel', {'-6', '-5', '-4', '-3', '-2', '-1', '0'})
set(gca, 'xtick', [1.5 2.5 3.5 4.5 5.5 6.5 7.5])

set(gca, 'tickdir', 'out')

saveas(gcf, 'seven', 'png')

quit
