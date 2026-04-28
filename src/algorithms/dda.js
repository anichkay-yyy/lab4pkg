import { measureAlgorithmTime } from '../utils/performance.js';

export const generateLinePoints = (x0, y0, x1, y1) => {
  const points = [];
  let dx = x1 - x0;
  let dy = y1 - y0;
  let steps = Math.max(Math.abs(dx), Math.abs(dy));
  if (steps === 0) {
    points.push([Math.round(x0), Math.round(y0)]);
    return points;
  }
  let xInc = dx / steps;
  let yInc = dy / steps;
  let x = x0;
  let y = y0;
  for (let i = 0; i <= steps; i++) {
    points.push([Math.round(x), Math.round(y)]);
    x += xInc;
    y += yInc;
  }
  return points;
};

export const computeLine = (x0, y0, x1, y1) => {
  const generate = () => generateLinePoints(x0, y0, x1, y1);
  const points = generate();
  const time = measureAlgorithmTime(generate);
  return { points, time, name: 'Алгоритм ЦДА' };
};
