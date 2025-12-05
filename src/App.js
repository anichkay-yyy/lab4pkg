import React, { useState, useCallback } from 'react';
import './App.css';

import Canvas from './components/Canvas';
import Controls from './components/Controls';
import Statistics from './components/Statistics';
import { computeLine as stepByStep } from './algorithms/stepByStep';
import { computeLine as dda } from './algorithms/dda';
import { computeLine as bresenhamLine } from './algorithms/bresenhamLine';
import { computeCircle as bresenhamCircle } from './algorithms/bresenhamCircle';

const ALGORITHMS = {
  stepByStep,
  dda,
  bresenhamLine,
  bresenhamCircle,
};

function App() {
  const [algorithm, setAlgorithm] = useState('stepByStep');
  const [showGrid, setShowGrid] = useState(true);
  const [lineInputs, setLineInputs] = useState({x1: 0, y1: 0, x2: 10, y2: 10});
  const [circleInputs, setCircleInputs] = useState({xc: 0, yc: 0, r: 10});
  const [selectedPoints, setSelectedPoints] = useState([]);
  const [currentResult, setCurrentResult] = useState(null);
  const [allResults, setAllResults] = useState([]);
  const [results, setResults] = useState([]);
  
  // Автоматическое определение режима на основе алгоритма
  const mode = algorithm === 'bresenhamCircle' ? 'circle' : 'line';

  const handleInputChange = useCallback((inputMode, field, value) => {
    if (inputMode === 'line') {
      setLineInputs(prev => ({...prev, [field]: value}));
    } else {
      setCircleInputs(prev => ({...prev, [field]: value}));
    }
  }, []);

  const handlePointSelect = useCallback((wx, wy) => {
    const newPoints = [...selectedPoints];
    if (newPoints.length >= 2) newPoints.length = 0;
    newPoints.push([wx, wy]);
    setSelectedPoints(newPoints);

    if (mode === 'line' && newPoints.length === 2) {
      setLineInputs({
        x1: newPoints[0][0], y1: newPoints[0][1],
        x2: newPoints[1][0], y2: newPoints[1][1]
      });
    } else if (mode === 'circle' && newPoints.length === 2) {
      const dx = newPoints[1][0] - newPoints[0][0];
      const dy = newPoints[1][1] - newPoints[0][1];
      const r = Math.round(Math.sqrt(dx*dx + dy*dy));
      setCircleInputs({ xc: newPoints[0][0], yc: newPoints[0][1], r });
    }
  }, [selectedPoints, mode]);

  const getParams = useCallback(() => {
    return mode === 'line' 
      ? [lineInputs.x1, lineInputs.y1, lineInputs.x2, lineInputs.y2]
      : [circleInputs.xc, circleInputs.yc, circleInputs.r];
  }, [mode, lineInputs, circleInputs]);

  const draw = useCallback(() => {
    const params = getParams();
    let result;
    
    if (mode === 'line') {
      const [x0, y0, x1, y1] = params;
      result = ALGORITHMS[algorithm](x0, y0, x1, y1);
    } else {
      const [xc, yc, r] = params;
      result = ALGORITHMS.bresenhamCircle(xc, yc, r);
    }
    
    setCurrentResult(result);
    setResults([result]);
    setAllResults([result]);
  }, [algorithm, mode, getParams]);

  const clear = useCallback(() => {
    setCurrentResult(null);
    setAllResults([]);
    setResults([]);
    setSelectedPoints([]);
  }, []);

  return (
    <div className="App">
      <header className="App-header">
        <h1>Растровые алгоритмы</h1>
        <p>Демонстрация с сеткой, осями и замерами времени</p>
      </header>

      <div className="main-content">
        <div className="left-panel">
          <Controls
            algorithm={algorithm}
            onAlgorithmChange={setAlgorithm}
            mode={mode}
            lineInputs={lineInputs}
            circleInputs={circleInputs}
            onInputChange={handleInputChange}
            selectedPoints={selectedPoints}
            onDraw={draw}
            onClear={clear}
            showGrid={showGrid}
            onGridChange={setShowGrid}
          />
          <Statistics currentResult={currentResult} allResults={allResults} />
        </div>

        <div className="canvas-container">
          <Canvas
            results={results}
            currentPoints={selectedPoints}
            onPointSelect={handlePointSelect}
            showGrid={showGrid}
          />
        </div>
      </div>
    </div>
  );
}

export default App;
