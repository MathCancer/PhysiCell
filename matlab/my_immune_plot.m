function MCDS = my_immune_plot( i )
filename = sprintf( 'output%08u.xml', i ); 
MCDS = read_MultiCellDS_xml( filename ); 
ind0 = find( MCDS.discrete_cells.metadata.type == 0 );
ind1 = find( MCDS.discrete_cells.metadata.type == 1 );

ind0_live = intersect( MCDS.discrete_cells.live_cells , ind0 );
ind0_dead = intersect( MCDS.discrete_cells.dead_cells , ind0 );

clf; 
simple_cutaway_plot( MCDS, ind0_live , 'r' );
simple_cutaway_plot( MCDS, ind0_dead , 'b' );
simple_cutaway_plot( MCDS, ind1 , 'g' );

axis( 500*[-1 1 -1 1 -1 1])

title( sprintf( '%3.2f days\n%u tumor cells (%3.2f%% live)\n%u immune cells' , MCDS.metadata.current_time / (60*24) , length(ind0), 100*length(ind0_live)/length(ind0), length(ind1)  ), 'fontsize', 14 )

xlabel( 'x (\mum)' , 'fontsize', 12 ); 
ylabel( 'y (\mum)' , 'fontsize', 12 ); 
zlabel( 'z (\mum)' , 'fontsize', 12 ); 
