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

function solution = Live_exact( parameters )

% allocate memory for the main outputs

solution.T = 0:parameters.dt:parameters.t_max; 
solution.A = zeros( 1 , length(solution.T)); 
solution.Live = zeros( 1 , length(solution.T)); 
solution.Total = zeros( 1 , length(solution.T)); 

% allocate memory for cell fractions

solution.AI = zeros(1,length(solution.T)); 

% get the main parameters 

b = parameters.L.birth_rate;
d = parameters.L.death_rate; 

TA = parameters.A.duration; 

% write out the mathematical model: 
% d[Populations]/dt = Operator*[Populations]

Operator = [ (b-d), 0; ...
    d , -1/TA ]; 

% eigenvectors and eigenvalues

[V,D] = eig(Operator);
eigenvalues = diag(D); 

% save the eigenvectors and eigenvalues in case you want them. 

solution.V = V; 
solution.D = D; 
solution.eigenvalues = eigenvalues; 

% initial condition 

VecNow = [ parameters.L.initial ; parameters.A.initial ] ; 
solution.Live(1) = VecNow(1); 
solution.A(1) = VecNow(2); 
solution.Total(1) = sum( VecNow(1:2) ); 

solution.AI(1) = solution.A(1) / solution.Total(1); 

% now, get the coefficients to write the analytic solution
% [Populations] = c1*V(:,1)*exp( d(1,1)*t) + c2*V(:,2)*exp( d(2,2)*t ) +
%                 c3*V(:,3)*exp( d(3,3)*t) + c4*V(:,4)*exp( d(4,4)*t ); 

coeff = linsolve( V , VecNow ); 

% find the (hopefully one) positive eigenvalue. 
% eigensolutions with negative eigenvalues decay, 
% leaving this as the long-time behavior. 

eigenvalues = diag(D); 
n = find( real( eigenvalues ) > 0 ); 
solution.long_time.AI = V(2,n) / sum( V(:,n) ) ;

% now, write out the solution at all the times 
for i=2:length( solution.T )
    % compact way to write the solution 
    VecExact = real( V*( coeff .* exp( eigenvalues*solution.T(i) ) ) ); 

    solution.A(i) = VecExact(2); 
    solution.Live(i) = VecExact(1);
    solution.Total(i) = sum( VecExact(1:2) ); 
   
    solution.AI(i) = solution.A(i) / solution.Total(i); 
end

return; 


