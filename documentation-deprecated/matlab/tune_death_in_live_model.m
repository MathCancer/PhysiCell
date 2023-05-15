% targets 

r = 0.04; 
AI = 0.02; 
TA = 8.6; 

% need: d; 
b = ( r + AI/TA)/(1-AI); 
b = round( b , 3 , 'significant' ); 
d = 0.01 * b; 

% set parameters 

parameters.dt = 0.1; % 6 min = 0.1 hours 
parameters.time_units = 'hour'; 
parameters.t_max = 3*24; % 3 days 


parameters.A.duration = 8.6; 
parameters.A.initial = 0; 

%TQ =   3.680494645500002
%d =   0.003117510255254
   

% set parameters 

parameters.dt = 0.1; % 6 min = 0.1 hours 
parameters.time_units = 'hour'; 
parameters.t_max = 3*24; % 3 days 

parameters.L.birth_rate = b; 
parameters.L.death_rate = d; 
parameters.L.initial = 1000; 

parameters.A.duration = 8.6; 
parameters.A.initial = 0; 

% run the exact solution 

solution = Live_exact( parameters ); 

relative_tolerance = 0.1;

step = 0.1; 

% steepest descent 
for k=1:300
    k 
    %params = [TQ,d]; 
    
    
    if( (solution.long_time.AI-AI)/AI > 0.0001 )
        %disp('too high');
        d = d*(1-step);
    end
    if( (AI-solution.long_time.AI)/AI > 0.0001 )
        %disp('too low'); 
        d = d*(1+step);
    end
    step = step*0.99; 
    
    parameters.L.death_rate = d; 
    solution = Live_exact( parameters ); 
    
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

coeff = polyfit( solution.T , log( solution.Total ) , 1 ); 
r_calc = coeff(1);
actual = [r_calc, solution.long_time.AI ]
goal = [r , AI  ]
abs( goal-actual)./actual 

b = round( b , 3 , 'significant' )
d = round( d , 3 , 'significant' )
