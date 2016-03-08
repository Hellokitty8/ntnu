data = importdata('25runstask3.txt');

subplot(2,1,1);
plot(data(:, 1), data(:, 2), 'LineWidth', 2);
title('Generation number of perfect solution found(200 == not found)');
subplot(2,1,2);
plot(data(:, 1), data(:, 3), 'LineWidth', 2);
title('Max Fitness Found (80 == perfect)');
