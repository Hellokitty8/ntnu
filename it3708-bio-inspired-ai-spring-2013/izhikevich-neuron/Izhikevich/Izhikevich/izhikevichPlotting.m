data = importdata('spiketrains.txt');

axis([0 1000 -80 80])
hold all
plot(data(1, :), 'LineWidth', 1)
hold all
plot(data(2, :), 'LineWidth', 1, 'Color', 'green')
xlabel('Time steps')
ylabel('Energy (mV)')
legend('Target Spike Train', 'Highest Fitness Spike Train');
