
import React, {useState, useLayoutEffect} from 'react';

<<<<<<< Updated upstream
const ControlPanel = ({ isAllowingNextStep, currentPlayer, gameOver, handleStep, handleRestart }) => {
  const [allowInput, setAllowInput] = useState(false);
  const setButtonText = isAllowingNextStep ? 'step' : 'wait';
  const nextPlayer = 1 + currentPlayer % 2;
  const currPlayer = gameOver ? nextPlayer : currentPlayer;
  const bgColor = currPlayer === 1? 'red': 'blue';
=======
const ControlPanel = ({isAllowingNextStep, currentPlayer, gameOver, handleStep, handleRestart}) => {
  // const [allowInput, setAllowInput] = useState(false);
  const setButtonText = isAllowingNextStep ? 'step' : 'wait';
  const nextPlayer = 1 + currentPlayer % 2;
  const _currentPlayer = gameOver ? nextPlayer: currentPlayer;
  const bgColor = isAllowingNextStep ?
  _currentPlayer === 1? 'red': 'blue' : 
  _currentPlayer === 1? 'red': 'blue';
>>>>>>> Stashed changes

  useLayoutEffect( () => {
    // to prevent a second click to get stored when main component stuck on heavy job
<<<<<<< Updated upstream
    if(!isAllowingNextStep) setAllowInput(false);
    setTimeout(() => {
      setAllowInput(isAllowingNextStep);
    }, 100)
  }, [isAllowingNextStep]);
=======
    // if(!props.isAllowingNextStep) setAllowInput(false);
    // setTimeout(() => {
    //   setAllowInput(props.isAllowingNextStep);
    // }, 100)
    console.log('props.isAllowingNextStep', isAllowingNextStep);
  }, [isAllowingNextStep]);

  useEffect( () => {
    console.log('ControlPanel render');
  });
>>>>>>> Stashed changes
  
  return (
    <div className={!isAllowingNextStep ? 'animate-flicker' : null}
      style={{ position:'absolute', height: 100, width: 100, right: -110, top: 0, backgroundColor: bgColor }}
    >
<<<<<<< Updated upstream
      {!gameOver && ( <button style={{marginTop: 20, cursor: 'pointer' }} disabled={!allowInput} 
=======
      {!gameOver && ( <button style={{marginTop: 20, cursor: 'pointer' }} disabled={!isAllowingNextStep} 
>>>>>>> Stashed changes
        onClick={handleStep}> 
        {setButtonText} </button>)}
      {gameOver && (<p style={{color:'white'}}>Game Over</p>)}
      {gameOver && ( <button style={{marginTop: 5}} onClick={handleRestart}> 
        restart </button>)}
    </div>
  );
};

export default ControlPanel;