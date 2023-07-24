% PhysiCell example - ODE vs. stochastic discrete models 
%
% Revised: February 19, 2018 (Version 1.0.0)
% 
% Created and copyrighted February 2018 by Paul Macklin: 
%    Paul.Macklin@MathCancer.org
%    http://MathCancer.org 
%
% GPL 2.0 or later license. 
% See: https://opensource.org/licenses/GPL-2.0
% 
% 

function solution = Cytometry_exact( parameters )

% allocate memory for the main outputs

solution.T = 0:parameters.dt:parameters.t_max; 

solution.G0G1 = zeros( 1 , length(solution.T)); 
solution.S = zeros( 1 , length(solution.T)); 
solution.G2M = zeros( 1 , length(solution.T)); 
solution.A = zeros( 1 , length(solution.T)); 

solution.Live = zeros( 1 , length(solution.T)); 
solution.Total = zeros( 1 , length(solution.T)); 

% allocate memory for cell fractions

solution.AI = zeros(1,length(solution.T)); 
solution.G0G1I = zeros(1,length(solution.T)); 
solution.SI = zeros(1,length(solution.T)); 
solution.G2MI = zeros(1,length(solution.T)); 

% get the main parameters 

TG0G1 = parameters.G0G1.duration; 
rG0G1A = parameters.G0G1.death_rate; 

TS = parameters.S.duration; 
rSA = parameters.S.death_rate; 

TG2M = parameters.G2M.duration; 
rG2MA = parameters.G2M.death_rate; 

TA = parameters.A.duration; 

% write out the mathematical model: 
% d[Populations]/dt = Operator*[Populations]

% [G0G1 , S , G2M , A] 

Operator = [ -(1/TG0G1 +rG0G1A) , 0 , 2/TG2M , 0; ...
    1/TG0G1 , -(1/TS + rSA) ,0 , 0; ...
    0 , 1/TS , -(1/TG2M + rG2MA) , 0; ... 
    rG0G1A , rSA, rG2MA , -1/TA ] ;

% eigenvectors and eigenvalues

[V,D] = eig(Operator);
eigenvalues = diag(D); 

% save the eigenvectors and eigenvalues in case you want them. 

solution.V = V; 
solution.D = D; 
solution.eigenvalues = eigenvalues; 

% initial condition 

VecNow = [ parameters.G0G1.initial ; parameters.S.initial ; ... 
    parameters.G2M.initial ; parameters.A.initial ] ; 

solution.G0G1(1) = VecNow(1); 
solution.S(1) = VecNow(2); 
solution.G2M(1) = VecNow(3); 
solution.A(1) = VecNow(4); 

% solution.K(1) = solution.K1(1) + solution.K2(1); 
solution.Live(1) = sum( VecNow(1:3) ); 
solution.Total(1) = sum( VecNow(1:4) ); 

solution.AI(1) = solution.A(1) / solution.Total(1); 
solution.G0G1I(1) = solution.G0G1(1) / solution.Total(1); 
solution.SI(1) = solution.S(1) / solution.Total(1); 
solution.G2MI(1) = solution.G2M(1) / solution.Total(1); 

% now, get the coefficients to write the analytic solution
% [Populations] = c1*V(:,1)*exp( d(1,1)*t) + c2*V(:,2)*exp( d(2,2)*t ) +
%                 c3*V(:,3)*exp( d(3,3)*t) + c4*V(:,4)*exp( d(4,4)*t ); 

coeff = linsolve( V , VecNow ); 

% find the (hopefully one) positive eigenvalue. 
% eigensolutions with negative eigenvalues decay, 
% leaving this as the long-time behavior. 

eigenvalues = diag(D); 
n = find( real( eigenvalues ) > 0 ); 

solution.long_time.G0G1I = V(1,n) / sum( V(:,n) ); 
solution.long_time.SI = V(2,n) / sum( V(:,n) ); 
solution.long_time.G2MI = V(3,n) / sum( V(:,n) ); 
solution.long_time.AI = V(4,n) / sum( V(:,n) ) ;

% now, write out the solution at all the times 
for i=2:length( solution.T )
    % compact way to write the solution 
    VecExact = real( V*( coeff .* exp( eigenvalues*solution.T(i) ) ) ); 

    solution.G0G1(i) = VecExact(1); 
    solution.S(i) = VecExact(2); 
    solution.G2M(i) = VecExact(3); 
    solution.A(i) = VecExact(4); 
    solution.Live(i) = sum( VecExact(1:3) ); 
    solution.Total(i) = sum( VecExact(1:4) ); 
   
    solution.AI(i) = solution.A(i) / solution.Total(i); 
    solution.G0G1I(i) = solution.G0G1(i) / solution.Total(i); 
    solution.SI(i) = solution.S(i) / solution.Total(i); 
    solution.G2MI(i) = solution.G2M(i) / solution.Total(i); 
end

return; 


