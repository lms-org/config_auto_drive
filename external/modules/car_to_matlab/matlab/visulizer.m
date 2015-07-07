%%%If you read that code, you have to refactor it and send a pull-request!
%Don't forget the TODOs!!!


%% TCP Data Receiver and Visualizer
% Receive tcp data
% Data is visualized via matlab plot.
%
% IMPORTANT: Do not send data in 100Hz frequency for live plotting! 
% Matlab is only able to plot live data at around 10Hz!
%
% TCP Data-Paket Format: <identifier1>:<value1>,<value2>,<value3>,...,<valueN>\n<identifier2>:<value1>,<value2>,<value3>,...,<valueN>\n...\n<identifierN>:<value1>,<value2>,<value3>,...,<valueN>\n
%TODO <identifier1>,<drawType>:<value1>,<value2>,<value3>
%drawType by default standard-plot, could be 2D with given x and some y-values, 3D etc.
clear all;
close all;
clc;


%% Connection Settings
HOSTIP = '127.0.0.1';       % IP Adress of Host ('0.0.0.0' = any)
PORT = 55555;               % Port of Host
%TODO use some ring-buffer to store data
SHOWXDATA = 1000;           % amount of datasets to be displayed in the plot
STOREMAXDATA = 100000;      % amount of datasets to be stored until script finishes




%% Get Data and visualize it

% connect to server
t = tcpip(HOSTIP, PORT, 'NetworkRole', 'client','InputBufferSize', 512, 'TransferDelay', 'off');
fopen(t);

% evaluate packet structure
packets = []; %Das sind keine Pakete sondern einfach ein read-Bufferb
max_val = 0;

%That code-block sucks
while isempty(packets)
    while t.BytesAvailable == 0
        pause(0.01)
    end
    raw = fread(t, t.BytesAvailable);
    fclose(t);
    packets = regexp(char(raw'), '\x{000}([\w\d\.\,\:_-]+\n)+', 'tokens');
    data_rows = regexp(cell2mat(packets{1,1}), '([\w\d_-]*):([\d\.,-]*)\n','tokens');
    %Scope?
    %allgemein sehr schlecht, da data_rows weder vollständing noch minimal sein muss!
    ReceivedData = cell(3,length(data_rows));
    for it_r = 1:length(data_rows)
        ReceivedData{1,it_r} = data_rows{1,it_r}{1,1}; %Setzt den namen?
        ReceivedData{2,it_r} = length(regexp(cell2mat(data_rows{1,it_r}), '([\d\.-]*),?', 'tokens'));%setzt die anzahl der variablen
    end
end

%that one either
% create matlab plots and initialize data
h_f = figure;
hold on;
h_pl = cell(1,size(ReceivedData,2));
for it_r = 1:size(ReceivedData,2)
    subplot(size(ReceivedData,2),1,it_r), hold on;
    for it_t=1:ReceivedData{2,it_r}
        h_pl{1,it_r}(it_t) = plot(1:SHOWXDATA, nan(SHOWXDATA,1));
    end
    title(ReceivedData{1,it_r});
    ReceivedData{3,it_r} = nan(STOREMAXDATA, ReceivedData{2,it_r});
end

 
 %TODO merge first two code-blocks into that one
 %verwirft unvollständige daten
 % get data and visualize it
 packet_count = 0;
 fopen(t);
  while packet_count<STOREMAXDATA
    while t.BytesAvailable == 0
        pause(0.000001);
    end
    raw = fread(t, t.BytesAvailable);
    packets = regexp(char(raw'), '\x{000}([\w\d\.\,\:_-]+\n)+', 'tokens');
    for it_p = 1:length(packets)
        packet_count = packet_count+1;
        %all rows with indentifier + data
        data_rows = regexp(cell2mat(packets{1,1}), '([\w\d_-]*):([\d\.,-]*)\n', 'tokens');
        for it_r = 1:length(data_rows)
            %one row with indentifier + data
            data_temp = regexp(cell2mat(data_rows{1,it_r}), '([\d\.-]*),?', 'tokens');
            for it_v = 1:length(data_temp)
                %convert to numbers
                ReceivedData{3,it_r}(packet_count,it_v) =  str2double(cell2mat(data_temp{it_v}));
                for it_t = 1:ReceivedData{2,it_r}
                    if packet_count <= SHOWXDATA
                        set(h_pl{1,it_r}(it_t), 'YData', ReceivedData{3,it_r}(1:SHOWXDATA,it_t));
                    else
                        set(h_pl{1,it_r}(it_t), 'YData', ReceivedData{3,it_r}(packet_count-SHOWXDATA+1:packet_count,it_t));
                    end
                end
            end
        end
        drawnow;
    end
  end
fclose(t);
pause(0.5);
