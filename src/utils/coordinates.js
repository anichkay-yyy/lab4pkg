export const screenToWorld = (screenX, screenY, canvasWidth, canvasHeight, scale) => {
  const centerX = canvasWidth / 2;
  const centerY = canvasHeight / 2;
  const worldX = (screenX - centerX) / scale;
  const worldY = (centerY - screenY) / scale; // Y направлен вверх
  return [worldX, worldY];
};

export const worldToScreen = (worldX, worldY, canvasWidth, canvasHeight, scale) => {
  const centerX = canvasWidth / 2;
  const centerY = canvasHeight / 2;
  const screenX = worldX * scale + centerX;
  const screenY = centerY - worldY * scale; // Y вверх
  return [screenX, screenY];
};
