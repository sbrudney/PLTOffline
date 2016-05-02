#plots values made from bin/PLTMC.cc
set terminal gif
set size 0.8, 0.8

set output "plot_ratio_tracks_equal.gif"
set title "Tracks with Triple Incidence Accepted and Rejected with Mask: Equal ROCs"
set xlabel "Side Length of Largest (Square) ROC (mm)"
set xrange [0.0:8.0]
set ylabel "% of Bad Triple Hits Rejected"
set yrange [0.0:1.1]
set y2label "% of Good Triple Hits Accepted"
set y2range [0.0:1.1]
plot "table_equal_badhits.txt" using 1:2 axes x1y1 title "Bad" with linespoints ,\
   "table_equal_goodhits.txt" using 1:2 axes x1y2 title "Good" with linespoints

set output "plot_ratio_tracks_cone.gif"
set title "Tracks with Triple Incidence Accepted and Rejected with Mask: Cone ROCs"





plot "table_cone_badhits.txt" using 1:2 title "Bad" with linespoints ,\
   "table_cone_goodhits.txt" using 1:2 title "Good" with linespoints

set output "plot_ratio_tracks_hourglass.gif"
set title "Tracks with Triple Incidence Accepted and Rejected with Mask: Hourglass ROCs"





plot "table_hourglass_badhits.txt" using 1:2 title "Bad" with linespoints ,\
   "table_hourglass_goodhits.txt" using 1:2 title "Good" with linespoints

set output "plot_ratio_tracks_cone_inverted.gif"
set title "Tracks with Triple Incidence Accepted and Rejected with Mask: Inverted Cone ROCs"





plot "table_cone_inverted_badhits.txt" using 1:2 title "Bad" with linespoints ,\
   "table_cone_inverted_goodhits.txt" using 1:2 title "Good" with linespoints


set output "plot_ratio_tracks_comparison.gif"
set title "Tracks with Triple Incidence Accepted and Rejected with Mask"





plot "table_equal_badhits.txt" using 1:2 title "Equal: Bad" with linespoints ,\
   "table_equal_goodhits.txt" using 1:2 title "Equal: Good" with linespoints, \
   "table_cone_badhits.txt" using 1:2 title "Cone: Bad" with linespoints ,\
   "table_cone_goodhits.txt" using 1:2 title "Cone: Good" with linespoints, \
   "table_hourglass_badhits.txt" using 1:2 title "Hourglass: Bad" with linespoints ,\
   "table_hourglass_goodhits.txt" using 1:2 title "Hourglass: Good" with linespoints, \
   "table_cone_inverted_badhits.txt" using 1:2 title "Cone Inv: Bad" with linespoints ,\
   "table_cone_inverted_goodhits.txt" using 1:2 title "Cone Inv: Good" with linespoints