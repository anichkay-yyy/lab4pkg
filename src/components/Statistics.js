import React from 'react';

const Statistics = ({ currentResult, allResults = [] }) => {
  if (!currentResult) {
    return <div className="stats">Статистика появится после отрисовки</div>;
  }

  const maxTime = Math.max(...allResults.map(r => r.time), currentResult.time || 0);

  return (
    <div className="stats">
      <h3>Текущий результат</h3>
      <p>Алгоритм: {currentResult.name}</p>
      <p>Время: {currentResult.time.toFixed(3)} мс (усреднено по 10000 запускам)</p>
      <p>Точек: {currentResult.points.length}</p>

      {allResults.length > 0 && (
        <>
          <h4>Сравнение производительности</h4>
          <table className="comparison-table">
            <thead>
              <tr>
                <th>Алгоритм</th>
                <th>Время (мс)</th>
                <th>Точек</th>
              </tr>
            </thead>
            <tbody>
              {allResults.map((result, idx) => (
                <tr key={idx}>
                  <td>{result.name}</td>
                  <td>{result.time.toFixed(3)}</td>
                  <td>{result.points.length}</td>
                </tr>
              ))}
            </tbody>
          </table>

          <div className="bar-chart">
            {allResults.map((result, idx) => (
              <div key={idx} className="bar-container">
                <div 
                  className="bar"
                  style={{ 
                    width: `${(result.time / maxTime * 100)}%`,
                    backgroundColor: `hsl(${idx * 90}, 70%, 50%)`
                  }}
                />
                <span>{result.name.slice(0,15)}: {result.time.toFixed(3)}мс</span>
              </div>
            ))}
          </div>
        </>
      )}
    </div>
  );
};

export default Statistics;
