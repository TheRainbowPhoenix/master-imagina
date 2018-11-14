package teamRaclette;

import java.util.ArrayList;
import java.util.Stack;


public class VUtils {

	public static float computeZCoordinate(Vector2 p1, Vector2 p2, Vector2 p3) {
		return p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y);
	}
	
	public static boolean isPointInsideTriangle(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 target) {
		float z1 = computeZCoordinate(p1, p2, target);
		float z2 = computeZCoordinate(p2, p3, target);
		float z3 = computeZCoordinate(p3, p1, target);
		return ((z1 > 0) && (z2 > 0) && (z3 > 0)) || ((z1 < 0) && (z2 < 0) && (z3 < 0));
	}
	
	/**
	 * 
	 * @param angle en degres
	 * @param dist
	 * @return
	 */
	public static Vector2 cartFromPolaire(double angle, double dist) {
		double rad = Math.toRadians(angle);
		return new Vector2((float) (-dist*Math.cos(rad)), (float) (dist*Math.sin(rad)));
	}
	
	/**
	 * 
	 * @param vec retourn teta en radians
	 * @return
	 */
	public static Vector2 polaireFromCart(Vector2 vec) {
		float teta = (float) Math.atan2(vec.y, vec.x);
		int distance = (int) Math.hypot(vec.x, vec.y);
		return new Vector2((float) Math.toDegrees(teta), (float) distance);
	}
	
	/**
	 * retourne la position voulue
	 */
	public static double setHeadingOn(Vector2 pos, Vector2 target) {
		Vector2 sortie = new Vector2(0,0);
		sortie.sub(pos);
		sortie.add(target);
		return(-sortie.angle());
	}
	
	public static Vector2 getPositionOfEntityFromMine(Vector2 myPosition, double angle, double distance) {
		Vector2 posCart = VUtils.cartFromPolaire(angle + 180, distance);
		posCart = posCart.add(myPosition);
		return posCart;
	}
	
	public static boolean isNextTo(Vector2 me, Vector2 target, double dist) {
		return (me.dst(target) < dist);
	}
	
	public static boolean isSame(Vector2 v1, Vector2 v2) {
		return (v1.dst(v2) < 1);
	}
	
	public static boolean containVector(Stack<Vector2> list, Vector2 v) {
		for (Vector2 i : list) {
			if (VUtils.isSame(v, i))
				return true;
		}
		return false;
	}
	
}
