% targets 

r = 0.04; 

AI = 0.02; 
TA = 8.6; 

TS = 8;
TG2 = 4; 
TM = 1; 
TG2M = TG2 + TM; 

% from live: 

d = 0.00319; 
b = 0.0432; 

% need: TG0G1 

% estimates 

TG0G1 = 1/b - TS - TG2 - TM; 

% set parameters 

parameters.dt = 0.1; % 6 min = 0.1 hours 
parameters.time_units = 'hour'; 
parameters.t_max = 3*24; % 3 days 

parameters.A.duration = TA; 
parameters.A.initial = 0; 

%TQ =   3.680494645500002
%d =   0.003117510255254
   

% set parameters 

parameters.dt = 0.1; % 6 min = 0.1 hours 
parameters.time_units = 'hour'; 
parameters.t_max = 3*24; % 3 days 

parameters.G0G1.duration = TG0G1; 
parameters.G0G1.death_rate = d; 
parameters.G0G1.initial = 1000; 

parameters.S.duration = TS; 
parameters.S.death_rate = d; 
parameters.S.initial = 0; 

parameters.G2M.duration = TG2M; 
parameters.G2M.death_rate = d; 
parameters.G2M.initial = 0; 

parameters.A.duration = TA; 
parameters.A.initial = 0; 

% run the exact solution 

solution = Cytometry_exact( parameters ); 

relative_tolerance = 0.1;

step = 0.1; 


% tune TQ

start_index = 200;
end_index = length( solution.T ); 

% steepest descent 
for k=1:300
    k 
    %params = [TQ,d]; 
    
    % only fit on later times, to avoid early dynamics effects 
    r_calc = polyfit( solution.T(start_index:end_index), log(solution.Total(start_index:end_index)) , 1 ); 
    r_calc = r_calc(1) 
    
    if( (r_calc-r)/r > 0.0001 )
        %disp('too high');
        TG0G1 = TG0G1*(1+step);
    end
    if( (r-r_calc)/r > 0.0001 )
        %disp('too low'); 
        TG0G1 = TG0G1*(1-step);
    end
    step = step*0.99; 
    
    parameters.G0G1.duration = TG0G1; 
    solution = Cytometry_exact( parameters ); 
    
    parameters.G0G1.initial = 1000*solution.long_time.G0G1I;
    parameters.S.initial = 1000*solution.long_time.SI;
    parameters.G2M.initial = 1000*solution.long_time.G2MI;
    parameters.A.initial = 1000*solution.long_time.AI;
    
    
    
    figure(1)
    clf
    plot( solution.T , solution.Total, 'r' );
    hold on 
    plot( solution.T , 1000*exp( r*solution.T ), 'b' ); 
    hold off
    title('pass sd (total)'); 
    
    figure(2)
    clf
    plot( solution.T , solution.A ./ solution.Total, 'r' );
    hold on 
    plot( solution.T , AI*ones(size(solution.T)), 'b' ); 
    hold off
    title('pass sd (apoptotic fraction)'); 

    pause(0.01)

    
%    step = step * .999; 
end

r_calc = polyfit( solution.T(start_index:end_index), log(solution.Total(start_index:end_index)) , 1 ); 
r_calc = r_calc(1); 
actual = [r_calc, solution.long_time.AI ]
goal = [r , AI  ]
abs( goal-actual)./actual 

b = round( b , 3 , 'significant' )
TG0G1 = round( TG0G1 , 3, 'significant' )
d = round( d , 3 , 'significant' )


