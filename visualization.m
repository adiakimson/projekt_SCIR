% Podstawowe informacje - zastąp danymi z własnego Thingspeak
channelID = 2342975;
readAPIKey = 'SCP5HEL2YFW2NIJG';

% Ustawienie timera do odświeżania co 30 sekund
t = timer('ExecutionMode', 'fixedRate', 'Period', 30, 'TimerFcn', {@refreshPlot, channelID, readAPIKey});

% Start timera
start(t);

% Funkcja odświeżająca wykres
function refreshPlot(~, ~, channelID, readAPIKey)
    % Odczytaj dane z Thingspeak
    data = thingSpeakRead(channelID, 'ReadKey', readAPIKey, 'NumPoints', 100);

    % Stwórz trójwymiarowy wykres
    figure;
    scatter3(data(:,1), data(:,2), data(:,3), 'filled', 'MarkerEdgeColor', 'b', 'MarkerFaceColor', 'b');

    % Dostosuj wykres
    xlabel('Oś X');
    ylabel('Oś Y');
    zlabel('Oś Z');
    title('Wizualizacja 3D z Akcelerometru');

    % Dodatkowe opcje dostosowania wykresu (opcjonalne)
    grid on;
    axis tight;
end