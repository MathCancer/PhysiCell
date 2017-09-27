function str = minutes_to_label( t )



days = floor( t / (24 * 60 )); 
t = t - 24*60*days; 

hours = floor( t/ 60); 
t= t - hours*60;  

minutes = t; 

str = sprintf('Current time: %i days, %i hours, and %3.2f minutes' , days, hours, minutes );
return; 