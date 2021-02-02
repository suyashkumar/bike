import './App.css';
import AppBar from '@material-ui/core/AppBar';
import Typography from '@material-ui/core/Typography';
import { withStyles, makeStyles } from '@material-ui/core/styles';
import Paper from '@material-ui/core/Paper';
import {Component} from "react";


const styles = theme => ({
  root: {
    flexGrow: 1,
  },
  header: {
    marginBottom: theme.spacing(10),
  },
  bar: {
    padding: theme.spacing(1),
  },
  rootPaper: {
    display: 'flex',
    flexWrap: 'wrap',
    marginLeft: 'auto',
    marginRight: 'auto',
    alignItems: 'center',
    justifyContent: 'center',
    width: 100,
    '& > *': {
      padding: theme.spacing(2),
    }
  },
});

// TODO(suyashkumar): refactor, cleanup, and extend.
class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      revs: 0,
    };
  }

  updateRevs = () => {
    fetch('http://10.0.0.176:8000/api/distance?session_id=dd023c2a-26b7-4798-a2ea-b71b964534f3',
      {method: 'GET', headers:{
          'Content-Type': 'text/plain',
        }})
      .then(res => res.text().then((txt) => {
        this.setState({revs: parseInt(txt.trim())})
        console.log(txt);
      }));
  };

  componentDidMount() {
    this.updateRevs();
    this.timer = setInterval(this.updateRevs, 100);
  }

  componentWillUnmount() {
    clearInterval(this.timer);
    this.timer = null;
  };

  render() {
    const classes = this.props.classes;
    return (
      <div className="App">
        <header className={classes.header}>
          <AppBar className={classes.bar}>
            <Typography variant="h6" className={classes.title}>
              🚴 Bike App
            </Typography>
          </AppBar>
        </header>
        <div className={classes.rootPaper}>
          <Paper elevation={3}>
            <Typography variant="h4" className={classes.title}>
              Revolutions: {this.state.revs}
            </Typography>
          </Paper>
        </div>
      </div>
    );
  }
}

export default withStyles(styles)(App);
