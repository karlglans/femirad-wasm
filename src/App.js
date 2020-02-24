import React, {useState, useEffect, useLayoutEffect } from 'react';
import './App.css';

import Grid from './components/Grid';
import ControlPanel from './components/ControlPanel';

import makeBoardArr, {allocBoardBuffer, copyBoardBuffer, markEmptyCell, releaseWasm, doNextMove} from './wasmdom/makeBoardArr';

const row = 16;

function AppHoc() {
  const [startedStep, setStartedStep] = useState(0);
  const [finishedStep, setFinishedStep] = useState(0);
  function _startStep() {
    setStartedStep(startedStep + 1);
    console.log('AppHoc:startStep()');
  }
  function _endStep() {
    setFinishedStep(startedStep);
    console.log('AppHoc:endStep()');
  }

  return (<App
    startedStep={startedStep}
    finishedStep={finishedStep}
    startStep={_startStep}
    endStep={_endStep}/>)
};

function App({ startedStep, finishedStep, startStep, endStep }) {
  const [isAllowingNextStep, setIsAllowingNextStep] = useState(true);
  const [currentPlayer, setCurrentPlayer] = useState(1);
  const [lastChangedCellIdx, setLastChangedCellIdx] = useState(-1);
  const [gameOver, setGameOver] = useState(false);
  const [boardArr, setBoardArr] = useState(makeBoardArr(16*16));
  const [startedStep0, setStartedStep0] = useState(0);
  const [finishedStep0, setFinishedStep0] = useState(0);

  function handleStep() {
<<<<<<< Updated upstream
    if (startedStep === finishedStep && isAllowingNextStep) {
      setIsAllowingNextStep(false);
    }
    setTimeout(() => {
      setStartedStep(startedStep + 1);
    }, 10);
=======
    setIsAllowingNextStep(false);
    setStartedStep0(startedStep0 + 1);
    setTimeout( ()=> {
      // startStep(true);
      setFinishedStep0(startedStep0)
    }, 100);
>>>>>>> Stashed changes
  }

  // useLayoutEffect
  useEffect( () => {
    if (startedStep0 > 0 && finishedStep0 === startedStep0) {
      console.log('App:useEffect()', startedStep, startedStep0)
      setStartedStep0(startedStep + 1);
      const change = doNextMove();
      const gameIsOver = change === -1;
      setCurrentPlayer(currentPlayer === 1? 2: 1);
      setGameOver(gameIsOver);
      setBoardArr(copyBoardBuffer());
      if (!gameIsOver) {
        setLastChangedCellIdx(change);
      }
      setTimeout( ()=> {
        setIsAllowingNextStep(true);
      }, 100);
  }}, [startedStep, startedStep0, currentPlayer, isAllowingNextStep, endStep]); // finishedStep, currentPlayer, isAllowingNextStep

  useEffect( () => {
    setTimeout(() => allocBoardBuffer(), 100);
    return () => releaseWasm();
  }, []);

  useEffect( () => {
    console.log('App render');
  });

  function handleClickCell(cellIdx) {
    if (isAllowingNextStep && !gameOver) {
      const gameIsOver = markEmptyCell(cellIdx);
      setCurrentPlayer(currentPlayer === 1? 2: 1); // maybe data from game instead
      setGameOver(gameIsOver);
      setBoardArr(copyBoardBuffer());
      // setFinishedStep(startedStep + 1);
      // setStartedStep(startedStep + 1);
    }
  }

  function handleRestart() {
    if (isAllowingNextStep) {
      setLastChangedCellIdx(-1);
    }
  }
  
  return (
    <div className="App" style={{display: 'flex', justifyContent: 'center', height: 'calc(100VH)', alignItems: 'center', backgroundColor: '#ebebeb' }}>
      <div style={{ position: 'relative', height: 600, width: 600 }}>
        <Grid
          row = {row}
          handleClickCell = {handleClickCell}
          lastChangedCellIdx = {lastChangedCellIdx}
          boardArr = {boardArr}
        />
        <ControlPanel
          gameOver={gameOver}
          handleStep={handleStep}
          handleRestart={handleRestart}
          isAllowingNextStep={isAllowingNextStep}
          // startedStep={startedStep}
          currentPlayer={currentPlayer}
        />
      </div>
    </div>
  );
}

export default AppHoc;
