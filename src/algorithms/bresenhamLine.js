import { measureAlgorithmTime } from '../utils/performance.js';

export const generateLinePoints = (x0, y0, x1, y1) => {
  const points = [];
  let dx = Math.abs(x1 - x0);
  let dy = Math.abs(y1 - y0);
  let sx = x0 < x1 ? 1 : -1;
  let sy = y0 < y1 ? 1 : -1;
  let err = dx - dy;

  let e2;
  let currentX = x0;
  let currentY = y0;

  while (true) {
    points.push([currentX, currentY]);
    if (currentX === x1 && currentY === y1) break;
    e2 = 2 * err;
    if (e2 > -dy) {
      err = err - dy;
      currentX = currentX + sx;
    }
    if (e2 < dx) {
      err = err + dx;
      currentY = currentY + sy;
    }
  }
  return points;
};

export const computeLine = (x0, y0, x1, y1) => {
  const generate = () => generateLinePoints(x0, y0, x1, y1);
  const points = generate();
  const time = measureAlgorithmTime(generate);
  return { points, time, name: 'Брезенхем (линия)' };
};
