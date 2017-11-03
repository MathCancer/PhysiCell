

base_command = 'magick mogrify -font Arial -fill black -pointsize 75 -gravity NorthWest -annotate +20+20'; 
xml_path = '../../cancer_immune_3D'; 

i = 0; 
t = 60*i; 

parfor i=0:1:3696
    xml_filename = sprintf( 'output%08u.xml' ,i );
    png_filename = sprintf( 'pov%08u.png' , i ); 
    MCDS = read_MultiCellDS_xml( xml_filename, xml_path ); 
    t = MCDS.metadata.current_time; 
    number_of_cells = length( MCDS.discrete_cells.metadata.type );
    label = sprintf( '%s\\n%i agents' , minutes_to_label(t) , number_of_cells ) ; 
    mycommand = sprintf('%s "%s" %s' , base_command , label , png_filename ); 
    disp( mycommand ); 
    system( mycommand )
    
end

% magick mogrify -font Arial -fill black -pointsize 75 -gravity NorthWest -annotate +20+20 @label.txt frame_in.png
% magick mogrify -font Arial -fill black -pointsize 75 -gravity NorthWest -annotate +20+20 @label.txt frame_in.png 
