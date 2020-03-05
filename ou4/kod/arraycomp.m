close all

figure
plot(aavgs(:, 1, 1, 1), aavgs(:, 2, 1, 1) ./ aavgs(:, 1, 1, 1))
hold on
plot(aavgs(:, 1, 3, 1), aavgs(:, 2, 3, 1) ./ aavgs(:, 1, 3, 1), 'LineWidth', 3)
title("Complexity for Insertion")
xlabel("Number of elements")
ylabel("Time in ms")
legend("Arraytable", "MTFtable", "Location", 'northwest')
hold off

figure
plot(aavgs(:, 1, 1, 2), aavgs(:, 2, 1, 2) ./ aavgs(:, 1, 1, 2))
hold on
plot(aavgs(:, 1, 3, 2), aavgs(:, 2, 3, 2) ./ aavgs(:, 1, 3, 2))
title("Complexity for Removal")
xlabel("Number of elements")
ylabel("Time in ms")
legend("Arraytable", "MTFtable", "Location", 'northwest')
hold off

figure
plot(aavgs(:, 1, 1, 3), aavgs(:, 2, 1, 3) ./ aavgs(:, 1, 1, 3))
hold on
plot(aavgs(:, 1, 3, 3), aavgs(:, 2, 3, 3) ./ aavgs(:, 1, 3, 3))
title("Complexity for Non-Existing lookup")
xlabel("Number of elements")
ylabel("Time in ms")
legend("Arraytable", "MTFtable", "Location", 'northwest')
hold off

figure
plot(aavgs(:, 1, 1, 4), aavgs(:, 2, 1, 4) ./ aavgs(:, 1, 1, 4))
hold on
plot(aavgs(:, 1, 3, 4), aavgs(:, 2, 3, 4) ./ aavgs(:, 1, 3, 4))
title("Complexity for Random Lookup")
xlabel("Number of elements")
ylabel("Time in ms")
legend("Arraytable", "MTFtable", "Location", 'northwest')
hold off

figure
plot(aavgs(:, 1, 1, 5), aavgs(:, 2, 1, 5) ./ aavgs(:, 1, 1, 5))
hold on
plot(aavgs(:, 1, 3, 5), aavgs(:, 2, 3, 5) ./ aavgs(:, 1, 3, 5))
title("Complexity for Skewed Lookup")
xlabel("Number of elements")
ylabel("Time in ms")
legend("Arraytable", "MTFtable", "Location", 'northwest')
hold off





