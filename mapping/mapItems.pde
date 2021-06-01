class MapItem {
  PVector pos; 
  ArrayList distances;

  MapItem(PVector pos, ArrayList distances) {
    this.pos = pos;
    this.distances = distances;
  }

  void draw(int halfSize) {
    for (int i = 0; i < distances.size(); i++) {
      if (distances.size() == 1) return;
      
      float theta = angleToPoint(i, distances.size());
      float alpha = angleToPoint(i + 1, distances.size());

      float distance = (float)distances.get(i);
      float nextDistance;

      if (distances.size() == i + 1) {
        nextDistance = (float)distances.get(0);
      } else {
        nextDistance = (float)distances.get(i + 1);
      }

      PVector p = PVector.fromAngle(theta);
      PVector q = PVector.fromAngle(alpha);

      p.setMag(distance * 2);
      q.setMag(nextDistance * 2);

      line(p.x + pos.x + halfSize, p.y + pos.y + halfSize, q.x + pos.x + halfSize, q.y + pos.y + halfSize);
    }
  }
}