close all
figure
hold on
%Plot random lookup for dlisttable
plot(aavgs(:, 1, 2, 4), aavgs(:, 2, 2, 4) ./ aavgs(:, 1, 2, 4)); 
%Plot random lookup for mtftable
plot(aavgs(:, 1, 3, 4), aavgs(:, 2, 3, 4) ./ aavgs(:, 1, 3, 4)); 
%none dlist
plot(aavgs(:, 1, 2, 3), aavgs(:, 2, 2, 3) ./ aavgs(:, 1, 2, 3), '--'); 
%none mtf
plot(aavgs(:, 1, 3, 3), aavgs(:, 2, 3, 3) ./ aavgs(:, 1, 3, 3), '--'); 

title("Non-existing- and random keys lookup comparison")
xlabel("Number of elements")
ylabel("Time in ms")
legend("Random lookup - dlist", "Random lookup - mtf", "Non-Existing - dlist", "Non-existing - mtf", "Location", "northwest")
hold off
figure
hold on
%skewed dlist
plot(aavgs(:, 1, 2, 5), aavgs(:, 2, 2, 5) ./ aavgs(:, 1, 2, 5)); 
%skewed mtf
plot(aavgs(:, 1, 3, 5), aavgs(:, 2, 3, 5) ./ aavgs(:, 1, 3, 5)); 
title("Skewed lookup comparison")
xlabel("Number of elements")
ylabel("Time in ms")
legend("Skewed - dlist", "Skewed - mtf", "Location", "northwest")
hold off
% figure
% hold on
% title("Time complexity for random lookup and skewed lookup using mtftable and dlisttable")
% xlabel("Number of elements")
% ylabel("Time in ms")
% legend("Non-Existing - dlist", "Non-existing - mtf", "Location", "northwest")
% hold off

