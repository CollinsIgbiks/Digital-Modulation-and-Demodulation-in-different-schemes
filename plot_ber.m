% Plots BER vs SNR for BPSK, QPSK and 16-QAM from the CSVs written by
% main.exe (ber_bpsk.csv, ber_qpsk.csv, ber_16qam.csv) and saves
% ber_vs_snr.png. Run main.exe first, then run this from the project folder.
%
% SNR is the value set by add_noise(): average power per stored double /
% per-dimension noise variance (Es/N0 for QPSK and 16-QAM, 2*Eb/N0 for BPSK).

schemes = {'bpsk', 'qpsk', '16qam'};
labels  = {'BPSK', 'QPSK', '16-QAM'};
markers = {'o', 's', '^'};
colors  = lines(numel(schemes));

fig = figure('Position', [100 100 900 600]);
theme(fig, 'light');  % R2025a+ defaults to dark; keep the PNG README-friendly
for k = 1:numel(schemes)
    data = readtable(['ber_' schemes{k} '.csv']);
    % Drop zero-BER points, which can't be shown on a log axis.
    measured = data(data.ber > 0, :);

    semilogy(measured.snr_db, measured.ber, markers{k}, ...
        'Color', colors(k, :), 'MarkerFaceColor', colors(k, :), ...
        'DisplayName', [labels{k} ' simulated']);
    hold on;
    semilogy(data.snr_db, data.theoretical_ber, '-', ...
        'Color', colors(k, :), 'LineWidth', 1.2, ...
        'DisplayName', [labels{k} ' theory']);
end
hold off;

grid on;
xlabel('SNR (dB)');
ylabel('Bit Error Rate');
title('BER vs SNR in AWGN');
legend('Location', 'southwest');
ylim([1e-5 1]);

exportgraphics(fig, 'ber_vs_snr.png', 'Resolution', 150);
fprintf('Saved ber_vs_snr.png\n');
