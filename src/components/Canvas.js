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
  scale = 20,
  onScaleChange,
  showGrid = true 
}) => {
  const canvasRef = useRef(null);
  const sizeRef = useRef({ width: 0, height: 0 });

  const draw = useCallback((ctx, width, height) => {
    ctx.fillStyle = '#ffffff';
    ctx.fillRect(0, 0, width, height);
    ctx.save();
    ctx.translate(width / 2, height / 2);
    ctx.scale(scale, -scale);
    ctx.lineWidth = Math.max(0.3 / scale, 0.05);
    ctx.strokeStyle = '#f5f5f5';
    ctx.fillStyle = '#000';

    const viewSize = Math.max(width, height) / scale * 0.8;
    const maxGridLines = 50;
    let gridStep = Math.pow(2, Math.floor(Math.log2(Math.max(1, viewSize / maxGridLines))));
    gridStep = Math.max(1, gridStep);
    const gridMin = Math.floor(-viewSize / gridStep) * gridStep;
    const gridMax = Math.ceil(viewSize / gridStep) * gridStep;

    if (showGrid) {
      // Основная сетка (светлая)
      ctx.strokeStyle = '#f0f0f0';
      ctx.lineWidth = Math.max(0.5 / scale, 0.05);
      for (let i = gridMin; i <= gridMax; i += gridStep) {
        ctx.beginPath();
        ctx.moveTo(i, gridMin);
        ctx.lineTo(i, gridMax);
        ctx.stroke();
        ctx.beginPath();
        ctx.moveTo(gridMin, i);
        ctx.lineTo(gridMax, i);
        ctx.stroke();
      }
      
      // Жирная сетка каждые 5 шагов
      ctx.strokeStyle = '#d0d0d0';
      ctx.lineWidth = Math.max(1 / scale, 0.1);
      for (let i = gridMin; i <= gridMax; i += gridStep * 5) {
        ctx.beginPath();
        ctx.moveTo(i, gridMin);
        ctx.lineTo(i, gridMax);
        ctx.stroke();
        ctx.beginPath();
        ctx.moveTo(gridMin, i);
        ctx.lineTo(gridMax, i);
        ctx.stroke();
      }
    }

    // Оси
    ctx.strokeStyle = '#999';
    ctx.lineWidth = Math.max(1.5 / scale, 0.2);
    ctx.lineCap = 'round';
    ctx.beginPath();
    ctx.moveTo(-viewSize, 0);
    ctx.lineTo(viewSize, 0);
    ctx.stroke();
    ctx.beginPath();
    ctx.moveTo(0, -viewSize);
    ctx.lineTo(0, viewSize);
    ctx.stroke();

    // Подписи осей (масштабируются вместе с сеткой)
    ctx.fillStyle = '#555';
    const fontSize = 12 / scale; // Масштабируется с сеткой, увеличенный размер
    ctx.font = `bold ${fontSize}px sans-serif`;
    
    // Шаг подписей = шаг жирной сетки (каждые 5*gridStep)
    const labelStep = gridStep * 5;
    const labelOffset = 25 / scale;

    // X-ось: снизу, горизонтально
    ctx.textAlign = 'center';
    ctx.textBaseline = 'top';
    for (let i = gridMin; i <= gridMax; i += labelStep) {
      if (i === 0) continue;
      ctx.save();
      ctx.scale(1, -1);
      ctx.fillText(i.toString(), i, labelOffset);
      ctx.restore();
    }

    // Y-ось: слева, горизонтально
    ctx.textAlign = 'right';
    ctx.textBaseline = 'middle';
    for (let i = gridMin; i <= gridMax; i += labelStep) {
      if (i === 0) continue;
      ctx.save();
      ctx.scale(1, -1);
      ctx.fillText(i.toString(), -labelOffset, -i);
      ctx.restore();
    }
    
    // Подпись начала координат (0,0) в углу
    ctx.textAlign = 'right';
    ctx.textBaseline = 'top';
    ctx.save();
    ctx.scale(1, -1);
    ctx.fillText('0', -labelOffset / 2, labelOffset / 2);
    ctx.restore();

    ctx.restore();

    // Выбранные точки
    ctx.fillStyle = '#FFD700';
    ctx.strokeStyle = '#B8860B';
    ctx.lineWidth = 2;
    currentPoints.forEach(([x, y]) => {
      const [sx, sy] = worldToScreen(x, y, width, height, scale);
      ctx.beginPath();
      ctx.arc(sx, sy, 10, 0, 2 * Math.PI);
      ctx.fill();
      ctx.stroke();
    });

    // Пиксели
    results.forEach(({ points, name }) => {
      const color = COLORS[name] || '#888';
      ctx.fillStyle = color;
      ctx.strokeStyle = color + '20';
      points.forEach(([wx, wy]) => {
        const [sx, sy] = worldToScreen(wx, wy, width, height, scale);
        const pixelSize = Math.max(scale * 0.7, 1);
        ctx.fillRect(sx - pixelSize/2, sy - pixelSize/2, pixelSize, pixelSize);
        ctx.strokeRect(sx - pixelSize/2, sy - pixelSize/2, pixelSize, pixelSize);
      });
    });
  }, [results, currentPoints, scale, showGrid]);

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

  const handleWheel = useCallback((e) => {
    e.preventDefault();
    const delta = e.deltaY > 0 ? 0.9 : 1.1;
    const newScale = Math.max(5, Math.min(100, scale * delta));
    onScaleChange(newScale);
  }, [scale, onScaleChange]);

  const handleClick = useCallback((e) => {
    const canvas = canvasRef.current;
    if (!canvas || !onPointSelect) return;
    const rect = canvas.getBoundingClientRect();
    const x = e.clientX - rect.left;
    const y = e.clientY - rect.top;
    const width = rect.width;
    const height = rect.height;
    const [wx, wy] = screenToWorld(x, y, width, height, scale);
    onPointSelect(Math.round(wx), Math.round(wy));
  }, [onPointSelect, scale]);

  return (
    <canvas
      ref={canvasRef}
      className="canvas"
      onWheel={handleWheel}
      onClick={handleClick}
      style={{ cursor: 'crosshair' }}
    />
  );
};

export default Canvas;
