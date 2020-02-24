
import React, {useState, useLayoutEffect} from 'react';

const ControlPanel = ({ isAllowingNextStep, currentPlayer, gameOver, handleStep, handleRestart }) => {
  const [allowInput, setAllowInput] = useState(false);
  const setButtonText = isAllowingNextStep ? 'step' : 'wait';
  const nextPlayer = 1 + currentPlayer % 2;
  const currPlayer = gameOver ? nextPlayer : currentPlayer;
  const bgColor = currPlayer === 1? 'red': 'blue';

  useLayoutEffect( () => {
    // to prevent a second click to get stored when main component stuck on heavy job
    if(!isAllowingNextStep) setAllowInput(false);
    setTimeout(() => {
      setAllowInput(isAllowingNextStep);
    }, 100)
  }, [isAllowingNextStep]);
  
  return (
    <div className={!allowInput ? 'animate-flicker' : null}
      style={{ position:'absolute', height: 100, width: 100, right: -110, top: 0, backgroundColor: bgColor }}
    >
      {!gameOver && ( <button style={{marginTop: 20, cursor: 'pointer' }} disabled={!allowInput} 
        onClick={handleStep}> 
        {setButtonText} </button>)}
      {gameOver && (<p style={{color:'white'}}>Game Over</p>)}
      {gameOver && ( <button style={{marginTop: 5}} onClick={handleRestart}> 
        restart </button>)}
    </div>
  );
};

export default ControlPanel;