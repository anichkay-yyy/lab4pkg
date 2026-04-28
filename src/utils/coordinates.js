export const screenToWorld = (screenX, screenY, canvasWidth, canvasHeight, scale = 20) => {
  const centerX = canvasWidth / 2;
  const centerY = canvasHeight / 2;
  const worldX = (screenX - centerX) / scale; // Учитываем масштаб
  const worldY = -(screenY - centerY) / scale; // Инвертируем Y и учитываем масштаб
  return [worldX, worldY];
};

export const worldToScreen = (worldX, worldY, canvasWidth, canvasHeight, scale = 20) => {
  const centerX = canvasWidth / 2;
  const centerY = canvasHeight / 2;
  const screenX = worldX * scale + centerX; // Учитываем масштаб
  const screenY = -worldY * scale + centerY; // Инвертируем Y и учитываем масштаб
  return [screenX, screenY];
};
