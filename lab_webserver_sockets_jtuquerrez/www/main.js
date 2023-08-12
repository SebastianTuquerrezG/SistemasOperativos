
let times = 0;

const link1_onClick = (event) =>{
    event.preventDefault();
    $(event.target).text(`Clicked on link ${times} times.`);
    times++;
    console.log('Clicked');
}

$(document).ready(() => {
    
    $("#link1").on("click", (event) => {
        link1_onClick(event);
    });
}
);