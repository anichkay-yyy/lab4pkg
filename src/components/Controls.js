import React from 'react';

const Controls = ({
  algorithm,
  onAlgorithmChange,
  mode,
  onModeChange,
  lineInputs,
  circleInputs,
  onInputChange,
  selectedPoints,
  onDraw,
  onClear,
  onCompareAll,
  showGrid,
  onGridChange
}) => {
  const handleSubmit = (e) => {
    e.preventDefault();
    onDraw();
  };

  const isLineMode = mode === 'line';

  return (
    <form onSubmit={handleSubmit} className="controls">
      <div className="control-group">
        <label>Алгоритм:</label>
        <select value={algorithm} onChange={(e) => onAlgorithmChange(e.target.value)}>
          <option value="stepByStep">Пошаговый алгоритм</option>
          <option value="dda">Алгоритм ЦДА</option>
          <option value="bresenhamLine">Брезенхем (линия)</option>
          <option value="bresenhamCircle">Брезенхем (окружность)</option>
        </select>
      </div>

      <div className="control-group">
        <label>
          <input
            type="radio"
            value="line"
            checked={mode === 'line'}
            onChange={(e) => onModeChange(e.target.value)}
          />
          Линия
        </label>
        <label>
          <input
            type="radio"
            value="circle"
            checked={mode === 'circle'}
            onChange={(e) => onModeChange(e.target.value)}
          />
          Окружность
        </label>
      </div>

      <div className="control-group">
        <label>
          <input
            type="checkbox"
            checked={showGrid}
            onChange={(e) => onGridChange(e.target.checked)}
          />
          Показать сетку
        </label>
      </div>

      {isLineMode ? (
        <div className="inputs-line">
          <div>
            <label>x1:</label>
            <input type="number" value={lineInputs.x1} onChange={(e) => onInputChange('line', 'x1', parseFloat(e.target.value) || 0)} />
          </div>
          <div>
            <label>y1:</label>
            <input type="number" value={lineInputs.y1} onChange={(e) => onInputChange('line', 'y1', parseFloat(e.target.value) || 0)} />
          </div>
          <div>
            <label>x2:</label>
            <input type="number" value={lineInputs.x2} onChange={(e) => onInputChange('line', 'x2', parseFloat(e.target.value) || 0)} />
          </div>
          <div>
            <label>y2:</label>
            <input type="number" value={lineInputs.y2} onChange={(e) => onInputChange('line', 'y2', parseFloat(e.target.value) || 0)} />
          </div>
        </div>
      ) : (
        <div className="inputs-circle">
          <div>
            <label>xc:</label>
            <input type="number" value={circleInputs.xc} onChange={(e) => onInputChange('circle', 'xc', parseFloat(e.target.value) || 0)} />
          </div>
          <div>
            <label>yc:</label>
            <input type="number" value={circleInputs.yc} onChange={(e) => onInputChange('circle', 'yc', parseFloat(e.target.value) || 0)} />
          </div>
          <div>
            <label>r:</label>
            <input type="number" value={circleInputs.r} onChange={(e) => onInputChange('circle', 'r', parseFloat(e.target.value) || 0)} />
          </div>
        </div>
      )}

      <div className="selected-points">
        <strong>Выбранные точки:</strong>
        {selectedPoints.map((point, idx) => (
          <div key={idx}>P{idx + 1}: ({point[0]}, {point[1]})</div>
        ))}
      </div>

      <div className="buttons">
        <button type="submit">Нарисовать</button>
        <button type="button" onClick={onClear}>Очистить</button>
        <button type="button" onClick={onCompareAll}>Сравнить все</button>
      </div>
    </form>
  );
};

export default Controls;
