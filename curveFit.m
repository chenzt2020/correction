clear
pureName= "2";
A = csvread(pureName+".csv");
x = A(:,1);
y = A(:,2);
y_ = A(:,3);
img = imread(pureName+".jpg");
tform = fitgeotrans([x y],[x y_],'polynomial',4);
imgOut = imwarp(img,tform,'OutputView',imref2d(size(img)));
imwrite(imgOut,pureName+"_m.jpg");