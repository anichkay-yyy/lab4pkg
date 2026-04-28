import React from 'react';

const Controls = ({
  algorithm,
  onAlgorithmChange,
  mode,
  lineInputs,
  circleInputs,
  onInputChange,
  selectedPoints,
  onDraw,
  onClear,
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
        <div className="algorithm-radios">
          <label>
            <input
              type="radio"
              value="stepByStep"
              checked={algorithm === 'stepByStep'}
              onChange={(e) => onAlgorithmChange(e.target.value)}
            />
            Пошаговый алгоритм
          </label>
          <label>
            <input
              type="radio"
              value="dda"
              checked={algorithm === 'dda'}
              onChange={(e) => onAlgorithmChange(e.target.value)}
            />
            Алгоритм ЦДА
          </label>
          <label>
            <input
              type="radio"
              value="bresenhamLine"
              checked={algorithm === 'bresenhamLine'}
              onChange={(e) => onAlgorithmChange(e.target.value)}
            />
            Брезенхем (линия)
          </label>
          <label>
            <input
              type="radio"
              value="bresenhamCircle"
              checked={algorithm === 'bresenhamCircle'}
              onChange={(e) => onAlgorithmChange(e.target.value)}
            />
            Брезенхем (окружность)
          </label>
        </div>
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
      </div>
    </form>
  );
};

export default Controls;
