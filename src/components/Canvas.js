import React, { useRef, useEffect, useCallback } from 'react';
import { screenToWorld, worldToScreen } from '../utils/coordinates';

const COLORS = {
  'Пошаговый алгоритм': '#FF6B6B',
  'Алгоритм ЦДА': '#4ECDC4',
  'Брезенхем (линия)': '#45B7D1',
  'Брезенхем (окружность)': '#96CEB4',
};

const Canvas = ({ 
  results = [], 
  currentPoints = [], 
  onPointSelect, 
  showGrid = true 
}) => {
  const canvasRef = useRef(null);
  const sizeRef = useRef({ width: 0, height: 0 });
  
  // Фиксированный масштаб: одна клетка = 20 пикселей
  const SCALE = 20;

  const draw = useCallback((ctx, width, height) => {
    ctx.fillStyle = '#ffffff';
    ctx.fillRect(0, 0, width, height);
    ctx.save();
    ctx.translate(width / 2, height / 2);
    ctx.scale(SCALE, SCALE); // Применяем фиксированный масштаб
    ctx.lineWidth = 0.3 / SCALE; // Толщина линий с учетом масштаба
    ctx.strokeStyle = '#f5f5f5';
    ctx.fillStyle = '#000';

    // Размер области отображения в мировых координатах
    const worldWidth = width / SCALE;
    const worldHeight = height / SCALE;
    const viewSize = Math.max(worldWidth, worldHeight) / 2;
    
    const maxGridLines = 50;
    let gridStep = Math.pow(2, Math.floor(Math.log2(Math.max(1, viewSize / maxGridLines))));
    gridStep = Math.max(1, gridStep);
    const gridMin = Math.floor(-viewSize / gridStep) * gridStep;
    const gridMax = Math.ceil(viewSize / gridStep) * gridStep;

    if (showGrid) {
      // Основная сетка (светлая)
      ctx.strokeStyle = '#f0f0f0';
      ctx.lineWidth = 0.5 / SCALE;
      for (let i = gridMin; i <= gridMax; i += gridStep) {
        ctx.beginPath();
        ctx.moveTo(i, -gridMax);
        ctx.lineTo(i, -gridMin);
        ctx.stroke();
        ctx.beginPath();
        ctx.moveTo(gridMin, -i);
        ctx.lineTo(gridMax, -i);
        ctx.stroke();
      }
      
      // Жирная сетка каждые 5 шагов
      ctx.strokeStyle = '#d0d0d0';
      ctx.lineWidth = 1 / SCALE;
      for (let i = gridMin; i <= gridMax; i += gridStep * 5) {
        ctx.beginPath();
        ctx.moveTo(i, -gridMax);
        ctx.lineTo(i, -gridMin);
        ctx.stroke();
        ctx.beginPath();
        ctx.moveTo(gridMin, -i);
        ctx.lineTo(gridMax, -i);
        ctx.stroke();
      }
    }

    // Оси
    ctx.strokeStyle = '#999';
    ctx.lineWidth = 1.5 / SCALE;
    ctx.lineCap = 'round';
    ctx.beginPath();
    ctx.moveTo(-viewSize, 0);
    ctx.lineTo(viewSize, 0);
    ctx.stroke();
    ctx.beginPath();
    ctx.moveTo(0, viewSize); // Инвертируем Y: от низа к верху
    ctx.lineTo(0, -viewSize);
    ctx.stroke();

    // Подписи осей 
    ctx.fillStyle = '#555';
    const fontSize = 12 / SCALE; // Размер шрифта с учетом масштаба
    ctx.font = `bold ${fontSize}px sans-serif`;
    
    // Шаг подписей = шаг жирной сетки (каждые 5*gridStep)
    const labelStep = gridStep * 5;
    const labelOffsetX = 25 / SCALE; // Отступ для X-оси (вниз)
    const labelOffsetY = 30 / SCALE; // Отступ для Y-оси (вправо)

    // X-ось: подписи снизу (под осью X, в отрицательной области Y)
    ctx.textAlign = 'center';
    ctx.textBaseline = 'bottom';
    for (let i = gridMin; i <= gridMax; i += labelStep) {
      if (i === 0) continue;
      ctx.fillText(i.toString(), i, labelOffsetX);
    }

    // Y-ось: подписи справа (положительные значения сверху, отрицательные снизу)
    ctx.textAlign = 'left';
    ctx.textBaseline = 'middle';
    for (let worldY = gridMin; worldY <= gridMax; worldY += labelStep) {
      if (worldY === 0) continue;
      const canvasY = -worldY; // Инвертируем: положительный worldY → верх (отрицательный canvasY)
      ctx.fillText(worldY.toString(), labelOffsetY, canvasY);
    }

    ctx.restore();

    // Выбранные точки
    ctx.fillStyle = '#FFD700';
    ctx.strokeStyle = '#B8860B';
    ctx.lineWidth = 2;
    currentPoints.forEach(([x, y]) => {
      const [screenX, screenY] = worldToScreen(x, y, width, height, SCALE);
      ctx.beginPath();
      ctx.arc(screenX, screenY, 10, 0, 2 * Math.PI);
      ctx.fill();
      ctx.stroke();
    });

    // Пиксели
    results.forEach(({ points, name }) => {
      const color = COLORS[name] || '#888';
      ctx.fillStyle = color;
      ctx.strokeStyle = color + '20';
      points.forEach(([wx, wy]) => {
        const [screenX, screenY] = worldToScreen(wx, wy, width, height, SCALE);
        ctx.fillRect(screenX - 3, screenY - 3, 6, 6);
        ctx.strokeRect(screenX - 3, screenY - 3, 6, 6);
      });
    });
  }, [results, currentPoints, showGrid]);

  const resize = useCallback(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    const rect = canvas.getBoundingClientRect();
    const dpr = window.devicePixelRatio || 1;
    canvas.width = rect.width * dpr;
    canvas.height = rect.height * dpr;
    sizeRef.current = { width: canvas.width, height: canvas.height };
    const ctx = canvas.getContext('2d');
    ctx.scale(dpr, dpr);
    draw(ctx, rect.width, rect.height);
  }, [draw]);

  useEffect(() => {
    window.addEventListener('resize', resize);
    resize();
    return () => window.removeEventListener('resize', resize);
  }, [resize]);

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    const ctx = canvas.getContext('2d');
    const rect = canvas.getBoundingClientRect();
    draw(ctx, rect.width, rect.height);
  }, [draw]);

  const handleClick = useCallback((e) => {
    const canvas = canvasRef.current;
    if (!canvas || !onPointSelect) return;
    const rect = canvas.getBoundingClientRect();
    const x = e.clientX - rect.left;
    const y = e.clientY - rect.top;
    const width = rect.width;
    const height = rect.height;
    const SCALE = 20; // Тот же масштаб, что и при отрисовке
    const [worldX, worldY] = screenToWorld(x, y, width, height, SCALE);
    onPointSelect(Math.round(worldX), Math.round(worldY));
  }, [onPointSelect]);

  return (
    <canvas
      ref={canvasRef}
      className="canvas"
      onClick={handleClick}
      style={{ cursor: 'crosshair' }}
    />
  );
};

export default Canvas;
