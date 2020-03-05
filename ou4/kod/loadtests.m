folders = ["arraytable_test_40000_10_5", "dlisttable_test_40000_10_5", "mtftable_test_40000_10_5"];
atests = zeros(50, 2, 3, 5);
aavgs = zeros(10, 2, 3, 5);

for j = 1:3
   for i = 1:5
      %atests(:, :, j, i) = importdata(strcat(string(folders(j)), "/split/t", num2str(i), "split.txt"));
      aavgs(:, :, j, i) = importdata(strcat(string(folders(j)), "/splitavg/t", num2str(i), "avg.txt"));
   end
end
