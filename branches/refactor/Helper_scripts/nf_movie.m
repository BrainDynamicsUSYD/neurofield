function nf_movie( nf, field, fname )
    % Script to generate a movie of a field with spatial variation
    % This script assumes the nodes are continuous, and the grid is a square
    %
    % nf_movie( nf, field, avi )
    %   field is a string of a field name e.g. "Propag.2.phi"
    %   avi is optionally a string filename to save the movie 
    %
    % Felix Fung 120322

    [data,side] = nf_grid(nf,field);
    [X,Y] = meshgrid(1:side,1:side);

    figure; h = surf( X, Y, data(:,:,1) );
	zlim([-5e-4 5e-4]);
    shading interp; lighting gouraud; camlight;
    F(nf.npoints) = getframe(gcf); % Trick to preallocate F

%     threshold = min(data(:));
%     data = data - threshold;
%     data = data/max(data(:));
    for t = 1:nf.npoints
       %threshold = min(min(data(:,:,t)));
        %pl = (data(:,:,t)-threshold)/max(max(data(:,:,t)-threshold));
        set( h, 'ZData', data(:,:,t)-mean(mean(data(:,:,t))) );
        title( ['Time: ',num2str(nf.deltat * t)] );
        F(t) = getframe(gcf);
    end
    
    % Output a movie if an avi filename was specified
    if ~(nargin < 3 || isempty(fname))
        avi = avifile(fname);
        avi = addframe( avi, F );
        avi = close(avi);
    end
end
