
bin_thickness = 0.1; 
bin_centers = 0.5*bin_thickness : bin_thickness : 2.0 - 0.5*bin_thickness; 
bin_edges = 0:bin_thickness:2; 

for i=0:216:1080
    filename = sprintf('output%08i.xml', i );
    MCDS = read_MultiCellDS_xml( filename ); 
    
    ind = MCDS.discrete_cells.live_cells; 
 
    m = mean( MCDS.discrete_cells.custom.oncoprotein(ind)); 
    s = std( MCDS.discrete_cells.custom.oncoprotein(ind)); 
    
    h = histogram( MCDS.discrete_cells.custom.oncoprotein(ind) , bin_edges ); 
    h.Normalization = 'probability';
    axis([0 2 0 0.5]);
    axis square; 

    text( .2 , .45 , sprintf('mean: %3.4f\ns.d.: %3.4f', m,s),'fontsize', 16 )

    
    xlabel('oncoprotein value', 'fontsize', 14 ); 
    ylabel('probability' ,'fontsize', 14 ); 
    str = sprintf( 'Oncoprotein distribution at %i days', i/24.0 ); 
    title( str ,'fontsize', 16);
    
    filename = sprintf( 'hist%08i.png' , i ); 
    print( '-dpng' , filename ); 
    filename = sprintf( 'hist%08i.eps' , i ); 
    print( '-deps' , filename ); 
%     filename = sprintf( 'hist%08i.pdf' , i ); 
%     print( '-dpdf' , filename ); 
    filename = sprintf( 'hist%08i.svg', i ); 
    print( '-dsvg' , filename ); 
    
end