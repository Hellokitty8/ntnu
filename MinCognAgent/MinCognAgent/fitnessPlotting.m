data = importdata('earesult.txt');

subplot(2,1,1);
line(data(:, 1), data(:, 2), 'LineWidth', 2)
line(data(:, 1), data(:, 3), 'LineWidth', 2, 'Color', 'green')
title('Max Fitness & Average Fitness')
subplot(2,1,2);
plot(data(:, 1), data(:, 4), 'LineWidth', 2, 'Color', 'red');
title('Std');