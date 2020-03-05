clear all;
g = @(n) n; %Predicted g(n) for Insertion
h = @(n) n; %Predicted g(n) for Removing
i = @(n) n; %Predicted g(n) for Worst case lookup
j = @(n) n; %Predicted g(n) for Random lookup
k = @(n) n; %Predicted g(n) for Skewed lookup

orderpreds = {1,2,3,4,5; g, h, i, j, k};

scriptinput = importdata("input.txt");

tests = zeros(scriptinput.data(2)*scriptinput.data(3), 2, 5);
avgs = zeros(scriptinput.data(2), 2, 5);

%Read data
for i = 1:5
   tests(:, :, i) = importdata(strcat("split/t", num2str(i), "split.txt"));
   avgs(:, :, i) = importdata(strcat("splitavg/t", num2str(i), "avg.txt"));
end

%Plot data per subtest with the average of each element amount teste. This result in the time complexity for each subtest.
figure('name',string(scriptinput.textdata))
hold on
for i= 1:5
    plot(avgs(:, 1, i), avgs(:, 2, i), 'LineWidth', 1); 
end
title("Complexity of test")
xlabel("Number of elements")
ylabel("Time in ms")
legend("Insert test", "Remove all test", "Non-existent lookup test", "Random lookup test", "Skewed lookup test", 'Location', 'northwest')
hold off


%Plot data per subtest divided with the element amount with the average of each element amount tested. This result in the time complexity for each the relevant table operation for each subtest.
figure('name',string(scriptinput.textdata))
hold on
for i = 1:5
    plot(avgs(:, 1, i), avgs(:, 2, i) ./ avgs(:, 1, i), 'LineWidth', 1); 
end
title("Complexity for operations")
xlabel("Number of elements")
ylabel("Time in ms")
legend("Insert test", "Remove all test", "Non-existent lookup test", "Random lookup test", "Skewed lookup test", 'Location', 'northwest')
hold off

%Plot the previous graph divided by a prediction (g(n)) set at the top of the code.
figure('name',string(scriptinput.textdata))
hold on
for i = 1:5
    t = orderpreds(i);
    plot(avgs(:, 1, i), avgs(:, 2, i) ./ avgs(:, 1, i) ./ avgs(:, 1, i), 'LineWidth', 1); 
end
title("Prediction of g(n)")
xlabel("Number of elements")
ylabel("Time in ms / g(n)")
legend("Insert test", "Remove all test", "Non-existent lookup test", "Random lookup test", "Skewed lookup test", 'Location', 'northwestoutside')
hold off

%Plots all runs for each subtest in the same graph. Produces a graph similar to the one from the complexity lecture.
figure('name',string(scriptinput.textdata))
hold on
for i = 1:5
    plot(tests(:, 1, i), tests(:, 2, i) ./ tests(:, 1, i), 'x'); 
end
title("Operation complexities for all runs")
xlabel("Number of elements")
ylabel("Time in ms")
legend("Insert test", "Remove all test", "Non-existent lookup test", "Random lookup test", "Skewed lookup test", 'Location', 'northwest')

