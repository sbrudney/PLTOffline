#plots values made from bin/PLTMC.cc
set terminal gif
set size 0.8, 0.8

set output "plot_ratio_tracks_equal.gif"
set title "Ratio of Tracks with Triple Incidence to Total Tracks: Equal ROCs"
set xlabel "Side Length of 1st (Square) ROC (mm)"
set xrange [2.0:6.5]
set ylabel "Ratio of Triple Hits/Total Hits"
set yrange [0.0:1.1]
plot "table_equal_badhits.txt" using 1:2 title "Bad" with linespoints ,\
   "table_equal_goodhits.txt" using 1:2 title "Good" with linespoints

set output "plot_ratio_tracks_cone.gif"
set title "Ratio of Tracks with Triple Incidence to Total Tracks: Cone ROCs"
set xlabel "Side Length of 1st (Square) ROC (mm)"
      set xrange [2.0:6.5]
set ylabel "Ratio of Triple Hits/Total Hits"
set yrange [0.0:1.1]
plot "table_cone_badhits.txt" using 1:2 title "Bad" with linespoints ,\
   "table_cone_goodhits.txt" using 1:2 title "Good" with linespoints

set output "plot_ratio_tracks_hourglass.gif"
set title "Ratio of Tracks with Triple Incidence to Total Tracks: Hourglass ROCs"
set xlabel "Side Length of 1st (Square) ROC (mm)"
      set xrange [2.0:6.5]
set ylabel "Ratio of Triple Hits/Total Hits"
set yrange [0.0:1.1]
plot "table_hourglass_badhits.txt" using 1:2 title "Bad" with linespoints ,\
   "table_hourglass_goodhits.txt" using 1:2 title "Good" with linespoints

set output "plot_ratio_tracks_cone_inverted.gif"
set title "Ratio of Tracks with Triple Incidence to Total Tracks: Inverted Cone ROCs"
set xlabel "Side Length of 3rd (Square) ROC (mm)"
      set xrange [2.0:6.5]
set ylabel "Ratio of Triple Hits/Total Hits"
set yrange [0.0:1.1]
plot "table_cone_inverted_badhits.txt" using 1:2 title "Bad" with linespoints ,\
   "table_cone_inverted_goodhits.txt" using 1:2 title "Good" with linespoints


set output "plot_ratio_tracks_comparison.gif"
set title "Ratio of Tracks with Triple Incidence to Total Tracks"
set xlabel "Side Length of Largest (Square) ROC (mm)"
      set xrange [2.0:6.5]
set ylabel "Ratio of Triple Hits/Total Hits"
set yrange [0.0:1.1]
plot "table_equal_badhits.txt" using 1:2 title "Equal: Bad" with linespoints ,\
   "table_equal_goodhits.txt" using 1:2 title "Equal: Good" with linespoints, \
   "table_cone_badhits.txt" using 1:2 title "Cone: Bad" with linespoints ,\
   "table_cone_goodhits.txt" using 1:2 title "Cone: Good" with linespoints, \
   "table_hourglass_badhits.txt" using 1:2 title "Hourglass: Bad" with linespoints ,\
   "table_hourglass_goodhits.txt" using 1:2 title "Hourglass: Good" with linespoints, \
   "table_cone_inverted_badhits.txt" using 1:2 title "Cone Inv: Bad" with linespoints ,\
   "table_cone_inverted_goodhits.txt" using 1:2 title "Cone Inv: Good" with linespoints