function worst = alterrcheck(gain,alt_err)

[rows,columns] = size(gain);

worst = 0;
for i = 1:columns,
   % Low altitude
   gain_intp = gain(1,i) + alt_err * (gain(2,i) - gain(1,i))/2980; 
   step = gain(2,i)-gain(1,i);
   gaindiff = gain_intp-gain(1,i);
   percent_err = (abs(gaindiff/step)) * 100;
   if abs(percent_err) > worst
      worst = abs(percent_err);
   end
   
   % Medium altitude
   gain_intp = gain(2,i) + alt_err * (gain(3,i) - gain(2,i))/3000; 
   step = gain(3,i)-gain(2,i);
   gaindiff = gain_intp-gain(2,i);
   percent_err = (abs(gaindiff/step)) * 100;
   if abs(percent_err) > worst
      worst = abs(percent_err);
   end
      
   % High altitude 
   gain_intp = gain(3,i) - alt_err * (gain(3,i) - gain(2,i))/3000; 
   step = gain(3,i)-gain(2,i);
   gaindiff = gain_intp-gain(3,i);
   percent_err = (abs(gaindiff/step)) * 100;
   if abs(percent_err) > worst
      worst = abs(percent_err);
   end
end   
   
   