import React, {useState} from 'react';

const style = { 
  flex: -1,
  width: 36,
  height: 36,
  backgroundColor: '#99af11', 
  display: 'flex',
  justifyContent: 'center',
  alignItems: 'center',
  borderRadius: 24,
  borderWidth: 1.5,
  cursor: 'pointer',
  color: '#fff'
};

function GridCell (props) {
  const [cellValue, setCellValue] = useState(props.cellState); 
  return (
    <div
      style={{ ...style, backgroundColor: props.cellState === 1 ? 'red' : props.cellState === 2 ? 'blue':'#aaa' }}
      onClick={() => props.clickhandler(props.cellIdx)}
      onMouseEnter={() => setCellValue(props.cellIdx)}
      onMouseLeave={() => setCellValue(props.cellState)} 
    >
      {props.flicker? null : cellValue}
    </div>
  );
}

function Grid (props) {
  return (
    <div style={{ display:'flex', flexFlow: 'row wrap', height: 600, width: 600 }}>
      {
        [...props.boardArr].map((cellState, i) => (
          <GridCell
            key={i}
            cellIdx={i}
            clickhandler={props.handleClickCell}
            cellState={cellState}
            flicker={props.lastChangedCellIdx === i}
          />
        ))
      }
    </div>
  );
}

export default Grid;