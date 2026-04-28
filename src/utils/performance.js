export const measureAlgorithmTime = (fn, iterations = 10000) => {
  let totalTime = 0;
  for (let i = 0; i < iterations; i++) {
    const start = performance.now();
    fn();
    totalTime += performance.now() - start;
  }
  return totalTime / iterations;
};
