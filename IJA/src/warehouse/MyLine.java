package warehouse;

import javafx.scene.paint.Color;

import java.util.List;

/**
 * Functions as a class for Line in the warehouse and stores the traffic and blockade pamameter
 */

public class MyLine {

    private int id;
    private List<Shelf> shelvesOnLine;
    private MyCoordinate start,end;
    private int traffic ;
    private Color color = Color.BLACK;
    private boolean blockade;



    private MyLine(){}

    public MyLine(int id, List<Shelf> shelvesOnLine, MyCoordinate start, MyCoordinate end, int traffic, boolean blockade){
        this.id = id;
        this.shelvesOnLine = shelvesOnLine;
        this.start = start;
        this.end = end;
        this.traffic = traffic;
        this.blockade = blockade;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void setShelvesOnLine(List<Shelf> shelvesOnLine) {
        this.shelvesOnLine = shelvesOnLine;
    }

    public List<Shelf> getShelvesOnLine() {
        return shelvesOnLine;
    }

    public void setStart(MyCoordinate start) {
        this.start = start;
    }

    public void setEnd(MyCoordinate end) {
        this.end = end;
    }

    public void setTraffic(int traffic) {
        this.traffic = traffic;
    }



    public void setBlockade(boolean blockade) {
        this.blockade = blockade;
    }
}
