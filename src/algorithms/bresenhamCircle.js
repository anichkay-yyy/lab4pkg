import { measureAlgorithmTime } from '../utils/performance.js';

export const generateCirclePoints = (xc, yc, r) => {
  const points = [];
  if (r === 0) {
    points.push([Math.round(xc), Math.round(yc)]);
    return points;
  }
  let x = 0;
  let y = r;
  let d = 3 - 2 * r;
  while (x <= y) {
    // 8 симметричных точек
    points.push([xc + x, yc + y]);
    points.push([xc - x, yc + y]);
    points.push([xc + x, yc - y]);
    points.push([xc - x, yc - y]);
    points.push([xc + y, yc + x]);
    points.push([xc - y, yc + x]);
    points.push([xc + y, yc - x]);
    points.push([xc - y, yc - x]);

    if (d <= 0) {
      d = d + 4 * x + 6;
      x++;
    } else {
      d = d + 4 * (x - y) + 10;
      x++;
      y--;
    }
  }
  return points;
};

export const computeCircle = (xc, yc, r) => {
  const generate = () => generateCirclePoints(xc, yc, r);
  const points = generate();
  const time = measureAlgorithmTime(generate);
  return { points, time, name: 'Брезенхем (окружность)' };
};
