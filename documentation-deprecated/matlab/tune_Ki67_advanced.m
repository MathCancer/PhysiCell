% targets 

r = 0.04; 
AI = 0.02; 
TA = 8.6; 
TK1 = 13;
TK2 = 2.5; 

% from live: 

d = 0.00319; 
b = 0.0432; 

% need: TQ 

% estimates 

TQ = 1/b - TK1 - TK2; 

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

parameters.K1.duration = TK1; 
parameters.K1.death_rate = d; 
parameters.K1.initial = 0; 

parameters.K2.duration = TK2; 
parameters.K2.death_rate = d; 
parameters.K2.initial = 0; 

parameters.Q.duration = TQ; 
parameters.Q.death_rate = d; 
parameters.Q.initial = 1000; 

parameters.A.duration = 8.6; 
parameters.A.initial = 0; 

% run the exact solution 

solution = Ki67_exact( parameters ); 

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
        TQ = TQ*(1+step);
    end
    if( (r-r_calc)/r > 0.0001 )
        %disp('too low'); 
        TQ = TQ*(1-step);
    end
    step = step*0.99; 
    
    parameters.Q.duration = TQ; 
    solution = Ki67_exact( parameters ); 
    
    parameters.Q.initial = 1000*solution.long_time.QI;
    parameters.K1.initial = 1000*solution.long_time.KI1;
    parameters.K2.initial = 1000*solution.long_time.KI2;
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
TQ = round( TQ , 3, 'significant' )
d = round( d , 3 , 'significant' )


