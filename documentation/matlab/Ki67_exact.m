% PhysiCell example - ODE vs. stochastic discrete models 
%
% Revised: September 8, 2016 (Version 1.0.0)
% 
% Created and copyrighted September 2016 by Paul Macklin: 
%    Paul.Macklin@MathCancer.org
%    http://MathCancer.org 
%
% GPL 2.0 or later license. 
% See: https://opensource.org/licenses/GPL-2.0
% 
% 

function solution = Ki67_exact( parameters )

% allocate memory for the main outputs

solution.T = 0:parameters.dt:parameters.t_max; 
solution.K1 = zeros( 1 , length(solution.T)); 
solution.K2 = zeros( 1 , length(solution.T)); 
solution.K = zeros( 1 , length(solution.T)); 
solution.Q = zeros( 1 , length(solution.T)); 
solution.A = zeros( 1 , length(solution.T)); 
solution.Live = zeros( 1 , length(solution.T)); 
solution.Total = zeros( 1 , length(solution.T)); 

% allocate memory for cell fractions

solution.AI = zeros(1,length(solution.T)); 
solution.KI1 = zeros(1,length(solution.T)); 
solution.KI2 = zeros(1,length(solution.T)); 
solution.KI = zeros(1,length(solution.T)); 

% get the main parameters 

T1 = parameters.K1.duration; 
r1A = parameters.K1.death_rate; 

T2 = parameters.K2.duration; 
r2A = parameters.K2.death_rate; 

TQ = parameters.Q.duration; 
rQA = parameters.Q.death_rate; 

TA = parameters.A.duration; 

% write out the mathematical model: 
% d[Populations]/dt = Operator*[Populations]

Operator = [ -(1/T1 +r1A) , 0 , 1/TQ , 0; ...
    2/T1 , -(1/T2 + r2A) ,0 , 0; ...
    0 , 1/T2 , -(1/TQ + rQA) , 0; ... 
    r1A , r2A, rQA , -1/TA ]; 

% eigenvectors and eigenvalues

[V,D] = eig(Operator);
eigenvalues = diag(D); 

% save the eigenvectors and eigenvalues in case you want them. 

solution.V = V; 
solution.D = D; 
solution.eigenvalues = eigenvalues; 

% initial condition 

VecNow = [ parameters.K1.initial ; parameters.K2.initial ; ... 
    parameters.Q.initial ; parameters.A.initial ] ; 
solution.K1(1) = VecNow(1); 
solution.K2(1) = VecNow(2); 
solution.Q(1) = VecNow(3); 
solution.A(1) = VecNow(4); 
solution.K(1) = solution.K1(1) + solution.K2(1); 
solution.Live(1) = sum( VecNow(1:3) ); 
solution.Total(1) = sum( VecNow(1:4) ); 

solution.AI(1) = solution.A(1) / solution.Total(1); 
solution.KI1(1) = solution.K1(1) / solution.Total(1); 
solution.KI2(1) = solution.K2(1) / solution.Total(1); 
solution.KI(1) = solution.KI1(1) + solution.KI2(1); 

% now, get the coefficients to write the analytic solution
% [Populations] = c1*V(:,1)*exp( d(1,1)*t) + c2*V(:,2)*exp( d(2,2)*t ) +
%                 c3*V(:,3)*exp( d(3,3)*t) + c4*V(:,4)*exp( d(4,4)*t ); 

coeff = linsolve( V , VecNow ); 

% find the (hopefully one) positive eigenvalue. 
% eigensolutions with negative eigenvalues decay, 
% leaving this as the long-time behavior. 

eigenvalues = diag(D); 
n = find( real( eigenvalues ) > 0 ); 
solution.long_time.KI1 = V(1,n) / sum( V(:,n) ); 
solution.long_time.KI2 = V(2,n) / sum( V(:,n) ); 
solution.long_time.QI = V(3,n) / sum( V(:,n) ); 
solution.long_time.AI = V(4,n) / sum( V(:,n) ) ;
solution.long_time.KI = solution.long_time.KI1 + solution.long_time.KI2; 

% now, write out the solution at all the times 
for i=2:length( solution.T )
    % compact way to write the solution 
    VecExact = real( V*( coeff .* exp( eigenvalues*solution.T(i) ) ) ); 

    solution.K1(i) = VecExact(1); 
    solution.K2(i) = VecExact(2); 
    solution.Q(i) = VecExact(3); 
    solution.A(i) = VecExact(4); 
    solution.K(i) = solution.K1(i) + solution.K2(i); 
    solution.Live(i) = sum( VecExact(1:3) ); 
    solution.Total(i) = sum( VecExact(1:4) ); 
   
    solution.AI(i) = solution.A(i) / solution.Total(i); 
    solution.KI1(i) = solution.K1(i) / solution.Total(i); 
    solution.KI2(i) = solution.K2(i) / solution.Total(i); 
    solution.KI(i) = solution.KI1(i) + solution.KI2(i);    
end

return; 


