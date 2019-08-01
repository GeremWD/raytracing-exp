
#include "Board.h"
#include <cmath>
using namespace LibBoard;
const int RAYS = 40;
int main( int, char *[] )
{
  Board board;
  board << Circle( 0, 0, 40, Color::Null, Color::Red, 0.0 );
  Line line( 0, 45, 0, 400, Color::Black, 1.0 );
  Line rline(line);
  const double angle = 2 * M_PI / RAYS;
  int n = RAYS;
  while ( n-- ) {
    board << rline;
    rline.rotate( angle, Point(0,0)  );
  }
  
  board.setClippingRectangle( -100, 100, 350, 250 );
  board.saveEPS( "flag.eps", 210, 297, 0.0, Board::UMillimeter );
  board.saveTikZ( "flag.tikz" );
  board.scaleToWidth(20,Board::UseLineWidth);
  board.saveSVG( "flag.svg", Board::BoundingBox, 0.0, Board::UCentimeter );
  board.saveEPS( "flag.eps", Board::BoundingBox, 0.0, Board::UCentimeter );

  return 0;
}