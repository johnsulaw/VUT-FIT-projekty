package warehouse;


import java.util.Objects;

public class MyCoordinate {

    private double coord_x;
    private double coord_y;

    /**
     * Default constructor for Jackson
     */
    public MyCoordinate() {}

    /**
     * Contructor of Coordinate
     * @param coord_x X coordinate
     * @param coord_y Y coordinate
     */
    public MyCoordinate(double coord_x, double coord_y) {
        this.coord_x = coord_x;
        this.coord_y = coord_y;
    }
    /**
     *
     * @param coord_x Coordinate X
     */
    public void setCoord_x(double coord_x) {
        this.coord_x = coord_x;
    }

    /**
     *
     * @param coord_y Coordinate Y
     */
    public void setCoord_y(double coord_y) {
        this.coord_y = coord_y;
    }

    /**
     *
     * @return Coordinate X
     */
    public double getCoord_x() {
        return coord_x;
    }

    /**
     *
     * @return Coordinate Y
     */
    public double getCoord_y() {
        return coord_y;
    }



    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        MyCoordinate that = (MyCoordinate) o;
        return Double.compare(that.getCoord_x(), getCoord_x()) == 0 &&
                Double.compare(that.getCoord_y(), getCoord_y()) == 0;
    }/*

    @Override
    public int hashCode() {
        return Objects.hash(getCoord_x(), getCoord_y());
    }*/

    @Override
    public String toString() {
        return "Coordinate{" +
                "x=" + coord_x +
                ", y=" + coord_y +
                '}';
    }

}
