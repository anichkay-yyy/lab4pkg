import { measureAlgorithmTime } from '../utils/performance.js';

export const generateLinePoints = (x0, y0, x1, y1) => {
  const points = [];
  const dx = x1 - x0;
  const dy = y1 - y0;
  const steps = Math.max(Math.abs(dx), Math.abs(dy));
  if (steps === 0) {
    points.push([Math.round(x0), Math.round(y0)]);
    return points;
  }
  for (let i = 0; i <= steps; i++) {
    const t = i / steps;
    const x = x0 + t * dx;
    const y = y0 + t * dy;
    points.push([Math.round(x), Math.round(y)]);
  }
  return points;
};

export const computeLine = (x0, y0, x1, y1) => {
  const generate = () => generateLinePoints(x0, y0, x1, y1);
  const points = generate();
  const time = measureAlgorithmTime(generate);
  return { points, time, name: 'Пошаговый алгоритм' };
};
