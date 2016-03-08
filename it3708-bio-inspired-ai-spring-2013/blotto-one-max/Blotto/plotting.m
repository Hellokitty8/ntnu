data = importdata('earesult.txt');

%Multiple sub graphs
figure
subplot(2,1,1)
line(data(:, 1), data(:, 2), 'LineWidth', 2)
line(data(:, 1), data(:, 3), 'LineWidth', 2, 'Color', 'green');
line(data(:, 1), data(:, 4), 'LineWidth', 2, 'Color', 'red');
title('Max Fitness, Average Fitness & Std');
subplot(2,1,2)
line(data(:, 1), data(:, 5), 'lineWidth', 2, 'Color', 'blue');
title('Avg. Strategic Entropy');


